#include "Sound.h"

// --------------------------------------------------------
// Initialize the underlying system
// --------------------------------------------------------
SoundImplement::SoundImplement()
{
	// Create the FMOD system
	m_soundSystem = NULL;
	Sound::ErrorCheck(FMOD::System_Create(&m_system));

	// Should we want to mix and update audio in FMOD Studio
	Sound::ErrorCheck(m_system->init(32, FMOD_STUDIO_INIT_LIVEUPDATE, NULL));
}

// --------------------------------------------------------
// Deconstructor
// --------------------------------------------------------
SoundImplement::~SoundImplement()
{
	Sound::ErrorCheck(m_system->release());
}

void SoundImplement::Update()
{
	// If a channel has stopped playing, we destroy it so 
	// we can clear up a channel to use
	vector<ChannelMap::iterator> pStoppedChannels;

	for (auto it = m_Channels.begin(), itEnd = m_Channels.end(); it != itEnd; ++it)
	{
		bool IsPlaying = false;
		it->second->isPlaying(&IsPlaying);
		if (!IsPlaying)
		{
			pStoppedChannels.push_back(it);
		}
	}

	for (auto& it : pStoppedChannels)
	{
		m_Channels.erase(it);
	}

	// Update the event sounds
	Sound::ErrorCheck(m_system->update());
}

SoundImplement* soundImplemenet = nullptr;

// --------------------------------------------------------
// Initializes the audio engine
// --------------------------------------------------------
void Sound::Init()
{
	// Subscribes event(s) here
	// eventBus->Subscribe(this, &Sound::<methodName>);

	soundImplemenet = new SoundImplement;
}

// --------------------------------------------------------
// Update the audio system
// --------------------------------------------------------
void Sound::Update()
{
	soundImplemenet->Update();
}

// --------------------------------------------------------
// Loads the selected sound file into the sound map
// --------------------------------------------------------
void Sound::LoadSound(const string& soundName, bool b_3d, bool b_Looping, bool b_Stream)
{
	// Grab the filename
	/// The 'auto' variables will allocate/deallocate automatically when the game flow enters and leaves its scope
	auto t_FoundIt = soundImplemenet->m_Sounds.find(soundName);
	if (t_FoundIt != soundImplemenet->m_Sounds.end())
		return;

	// Determines if the file is in a 3D space, looping, and streaming
	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= b_3d ? FMOD_3D : FMOD_2D;
	eMode |= b_Looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= b_Stream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	// Store it into the Sound map
	FMOD::Sound* pSound = nullptr;
	Sound::ErrorCheck(soundImplemenet->m_system->createSound(soundName.c_str(), eMode, nullptr, &pSound));
	if (pSound)
	{
		soundImplemenet->m_Sounds[soundName] = pSound;
	}
}

// --------------------------------------------------------
// Unloads the specific file from the sound map
// --------------------------------------------------------
void Sound::UnloadSound(const string& soundName)
{
	auto t_FoundIt = soundImplemenet->m_Sounds.find(soundName);
	if (t_FoundIt == soundImplemenet->m_Sounds.end())
		return;

	Sound::ErrorCheck(t_FoundIt->second->release());
	soundImplemenet->m_Sounds.erase(t_FoundIt);
}

// --------------------------------------------------------
// Plays the file from the audio map
// --------------------------------------------------------
int Sound::PlaySounds(const string& soundName, const Vector3& pos, float volumedB)
{
	// Check if sound is in our Sound map
	int channelId = soundImplemenet->m_nextChannelId++;
	auto t_FoundIt = soundImplemenet->m_Sounds.find(soundName);
	if (t_FoundIt == soundImplemenet->m_Sounds.end())
	{
		// If it's not found, then it's loaded into the map
		LoadSound(soundName);
		t_FoundIt = soundImplemenet->m_Sounds.find(soundName);
		
		// If it's still not found, then we don't load
		if (t_FoundIt == soundImplemenet->m_Sounds.end())
		{
			return channelId;
		}
	}

	// Create a new channel
	FMOD::Channel* pChannel = nullptr;
	Sound::ErrorCheck(soundImplemenet->m_system->playSound(t_FoundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		// Once all the attributes are set, play sound
		FMOD_MODE currMode;
		t_FoundIt->second->getMode(&currMode);
		if (currMode & FMOD_3D)
		{
			FMOD_VECTOR position = VectorToFmod(pos);
			FMOD_VECTOR velocity = { (pos.x - position.x), (pos.y - position.y), (pos.z - position.z) };
			//FMOD_VECTOR direction = { 1.0f, 2.0f, 3.0f };
			Sound::ErrorCheck(pChannel->set3DAttributes(&position, &velocity));
			//Sound::ErrorCheck(pChannel->set3DConeOrientation(&direction));
			//Sound::ErrorCheck(pChannel->set3DConeSettings(30.0f, 60.0f, 0.5f));
			//Sound::ErrorCheck(pChannel->set3DMinMaxDistance(1.0f, 1000.0f));
		}
		Sound::ErrorCheck(pChannel->setVolume(dbToVolume(volumedB)));
		Sound::ErrorCheck(pChannel->setPaused(false));
		soundImplemenet->m_Channels[channelId] = pChannel;
	}
	return channelId;
}

void Sound::Set3dListenerAndOrientation(const Vector3& pos, const Vector3& velocity, const Vector3& forward, const Vector3& up) 
{
	FMOD_VECTOR v3_position = VectorToFmod(pos);
	FMOD_VECTOR v3_velocity = VectorToFmod(velocity);
	FMOD_VECTOR v3_forward = VectorToFmod(forward);
	FMOD_VECTOR v3_up = VectorToFmod(up);

	soundImplemenet->m_system->set3DListenerAttributes(0, &v3_position, &v3_velocity, &v3_forward, &v3_up);
}

// --------------------------------------------------------
// Moves the channel's positioning in the 3D space
// --------------------------------------------------------
void Sound::SetChannel3dPosition(int channelId, const Vector3& pos)
{
	auto t_FoundIt = soundImplemenet->m_Channels.find(channelId);
	if (t_FoundIt == soundImplemenet->m_Channels.end())
		return;

	FMOD_VECTOR position = VectorToFmod(pos);
	Sound::ErrorCheck(t_FoundIt->second->set3DAttributes(&position, NULL));
}

// --------------------------------------------------------
// Sets the volume of the specified channel
// --------------------------------------------------------
void Sound::SetChannelVolume(int channelId, float volumedB)
{
	auto t_FoundIt = soundImplemenet->m_Channels.find(channelId);
	if (t_FoundIt == soundImplemenet->m_Channels.end())
		return;

	Sound::ErrorCheck(t_FoundIt->second->setVolume(dbToVolume(volumedB)));
}

// --------------------------------------------------------
// Stores all the sounds and info for each event
// --------------------------------------------------------
void Sound::LoadBank(const string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
{
	// Grab the filename
	auto t_FoundIt = soundImplemenet->m_Banks.find(bankName);
	if (t_FoundIt != soundImplemenet->m_Banks.end())
		return;

	// Store it into the Bank map
	FMOD::Studio::Bank* pBank;
	Sound::ErrorCheck(soundImplemenet->m_soundSystem->loadBankFile(bankName.c_str(), flags, &pBank));
	if (pBank)
	{
		soundImplemenet->m_Banks[bankName] = pBank;
	}
}

// --------------------------------------------------------
// Loads events separately into the map
// --------------------------------------------------------
void Sound::LoadEvent(const string& eventName)
{
	// Grab the filename
	auto tFoundit = soundImplemenet->m_Events.find(eventName);
	if (tFoundit != soundImplemenet->m_Events.end())
		return;

	// Load the description of the event
	FMOD::Studio::EventDescription* pEventDescription = NULL;
	Sound::ErrorCheck(soundImplemenet->m_soundSystem->getEvent(eventName.c_str(), &pEventDescription));

	// Create the instance of the event
	if (pEventDescription)
	{
		FMOD::Studio::EventInstance* pEventInstance = NULL;
		Sound::ErrorCheck(pEventDescription->createInstance(&pEventInstance));
		if (pEventInstance)
		{
			soundImplemenet->m_Events[eventName] = pEventInstance;
		}
	}
}

// --------------------------------------------------------
// Plays the event coming from the event map
// --------------------------------------------------------
void Sound::PlayEvent(const string &eventName)
{
	// Checks if the event is there
	auto tFoundit = soundImplemenet->m_Events.find(eventName);
	if (tFoundit == soundImplemenet->m_Events.end())
	{
		// If it's not there, load it
		LoadEvent(eventName);
		tFoundit = soundImplemenet->m_Events.find(eventName);
		if (tFoundit == soundImplemenet->m_Events.end())
			return;
	}
	tFoundit->second->start();
}

// --------------------------------------------------------
// Stops the event from playing
// --------------------------------------------------------
void Sound::StopEvent(const string &eventName, bool bImmediate)
{
	auto t_FoundIt = soundImplemenet->m_Events.find(eventName);
	if (t_FoundIt == soundImplemenet->m_Events.end())
		return;

	// Determines if we want to stop it immediately or fade it out
	FMOD_STUDIO_STOP_MODE eMode;
	eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
	Sound::ErrorCheck(t_FoundIt->second->stop(eMode));
}

bool Sound::IsPlaying(int channelId) const
{
	//auto t_FoundIt = soundImplemenet->m_Channels.find(channelId);
	//if (t_FoundIt == soundImplemenet->m_Channels.end())
	//	return false;

	//// If it is playing, return its state to such
	//FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
	//if (t_FoundIt->second->isPlaying == FMOD_STUDIO_PLAYBACK_PLAYING)
	//{
	//	return true;
	//}
	return false;
}

// --------------------------------------------------------
// Checks if the event is playing
// --------------------------------------------------------
bool Sound::IsEventPlaying(const string &eventName) const
{
	auto t_FoundIt = soundImplemenet->m_Events.find(eventName);
	if (t_FoundIt == soundImplemenet->m_Events.end())
		return false;

	// If it is playing, return its state to such
	FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
	if (t_FoundIt->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) 
	{
		return true;
	}
	return false;
}

// --------------------------------------------------------
// Returns the event parameter on the proper event
// --------------------------------------------------------
void Sound::GetEventParameter(const string &eventName, const string &parameterName, float* parameter)
{
	auto t_FoundIt = soundImplemenet->m_Events.find(eventName);
	if (t_FoundIt == soundImplemenet->m_Events.end())
		return;

	FMOD::Studio::ParameterInstance* pParameter = NULL;
	Sound::ErrorCheck(t_FoundIt->second->getParameter(parameterName.c_str(), &pParameter));
	Sound::ErrorCheck(pParameter->getValue(parameter));
}

// --------------------------------------------------------
// Set up the event parameter dynamically
// --------------------------------------------------------
void Sound::SetEventParameter(const string &eventName, const string &parameterName, float value)
{
	auto t_FoundIt = soundImplemenet->m_Events.find(eventName);
	if (t_FoundIt == soundImplemenet->m_Events.end())
		return;

	FMOD::Studio::ParameterInstance* pParameter = NULL;
	Sound::ErrorCheck(t_FoundIt->second->getParameter(parameterName.c_str(), &pParameter));
	Sound::ErrorCheck(pParameter->setValue(value));
}

// --------------------------------------------------------
// Stops channel given the channel ID
// --------------------------------------------------------
void Sound::StopChannel(int channelId)
{
	auto t_FoundIt = soundImplemenet->m_Channels.find(channelId);
	if (t_FoundIt == soundImplemenet->m_Channels.end())
		return;

	Sound::ErrorCheck(soundImplemenet->m_Channels[channelId]->stop());
}

// --------------------------------------------------------
// Stops all channels in the channel map
// --------------------------------------------------------
void Sound::StopAllChannels()
{
	/*for (int i = 0; i < soundImplemenet->m_Channels.count; i++) 
	{
		auto t_FoundIt = soundImplemenet->m_Channels.find(i);
		if (t_FoundIt == soundImplemenet->m_Channels.end())
			return;

		Sound::ErrorCheck(soundImplemenet->m_Channels[i]->stop());
	}*/
}

// --------------------------------------------------------
// Convert Vector to FMOD's Vector3
// --------------------------------------------------------
FMOD_VECTOR Sound::VectorToFmod(const Vector3& pos)
{
	FMOD_VECTOR fVec;
	fVec.x = pos.x;
	fVec.y = pos.y;
	fVec.z = pos.z;
	return fVec;
}

// --------------------------------------------------------
// Volume --> dB
// --------------------------------------------------------
float  Sound::dbToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

// --------------------------------------------------------
// dB --> Volume
// --------------------------------------------------------
float  Sound::VolumeTodB(float volume)
{
	return 20.0f * log10f(volume);
}

// --------------------------------------------------------
// Checks that all FMOD calls are successful
// --------------------------------------------------------
int Sound::ErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		cout << "FMOD ERROR: " << result << endl;
		return 1;
	}
	// cout << "No Worries from FMOD" << endl;
	return 0;
}

// --------------------------------------------------------
// Deletes the engine to prevent memory leaks
// --------------------------------------------------------
void Sound::Shutdown()
{
	delete soundImplemenet;
}