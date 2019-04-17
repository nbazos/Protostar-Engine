#include "Sound.h"

SoundImplement::SoundImplement()
{
	m_soundSystem = NULL;
	Sound::ErrorCheck(FMOD::Studio::System::create(&m_soundSystem));
	Sound::ErrorCheck(m_soundSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

	m_system = NULL;
	Sound::ErrorCheck(m_soundSystem->getLowLevelSystem(&m_system));
}

SoundImplement::~SoundImplement()
{
	Sound::ErrorCheck(m_soundSystem->unloadAll());
	Sound::ErrorCheck(m_soundSystem->release());
}

void SoundImplement::Update() 
{
	vector<ChannelMap::iterator> pStoppedChannels;
	for (auto it = m_Channels.begin(), itEnd = m_Channels.end(); it != itEnd; ++it)
	{
		bool bIsPlaying = false;
		it->second->isPlaying(&bIsPlaying);
		if (!bIsPlaying)
		{
			pStoppedChannels.push_back(it);
		}
	}
	for (auto& it : pStoppedChannels)
	{
		m_Channels.erase(it);
	}
	Sound::ErrorCheck(m_soundSystem->update());
}

SoundImplement* audioImplemenet = nullptr;

void Sound::Init() 
{
	audioImplemenet = new SoundImplement;
}

void Sound::Update() {
	audioImplemenet->Update();
}

void Sound::LoadSound(const std::string& soundName, bool b_3d, bool b_Looping, bool b_Stream)
{
	auto tFoundIt = audioImplemenet->m_Sounds.find(soundName);
	if (tFoundIt != audioImplemenet->m_Sounds.end())
		return;
	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= b_3d ? FMOD_3D : FMOD_2D;
	eMode |= b_Looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= b_Stream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
	FMOD::Sound* pSound = nullptr;
	Sound::ErrorCheck(audioImplemenet->m_system->createSound(soundName.c_str(), eMode, nullptr, &pSound));
	if (pSound) 
	{
		audioImplemenet->m_Sounds[soundName] = pSound;
	}
}

void Sound::UnLoadSound(const std::string& soundName)
{
	auto tFoundIt = audioImplemenet->m_Sounds.find(soundName);
	if (tFoundIt == audioImplemenet->m_Sounds.end())
		return;
	Sound::ErrorCheck(tFoundIt->second->release());
	audioImplemenet->m_Sounds.erase(tFoundIt);
}

int Sound::PlaySounds(const string& soundName, const Vector3& pos, float volumedB)
{
	int channelId = audioImplemenet->m_nextChannelId++;
	auto tFoundIt = audioImplemenet->m_Sounds.find(soundName);
	if (tFoundIt == audioImplemenet->m_Sounds.end())
	{
		LoadSound(soundName);
		tFoundIt = audioImplemenet->m_Sounds.find(soundName);
		if (tFoundIt == audioImplemenet->m_Sounds.end())
		{
			return channelId;
		}
	}
	FMOD::Channel* pChannel = nullptr;
	Sound::ErrorCheck(audioImplemenet->m_system->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		FMOD_MODE currMode;
		tFoundIt->second->getMode(&currMode);
		if (currMode & FMOD_3D) 
		{
			FMOD_VECTOR position = VectorToFmod(pos);
			Sound::ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
		}
		Sound::ErrorCheck(pChannel->setVolume(dbToVolume(volumedB)));
		Sound::ErrorCheck(pChannel->setPaused(false));
		audioImplemenet->m_Channels[channelId] = pChannel;
	}
	return channelId;
}

void Sound::SetChannel3dPosition(int channelId, const Vector3& pos)
{
	auto tFoundIt = audioImplemenet->m_Channels.find(channelId);
	if (tFoundIt == audioImplemenet->m_Channels.end())
		return;

	FMOD_VECTOR position = VectorToFmod(pos);
	Sound::ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}

void Sound::SetChannelVolume(int channelId, float volumedB)
{
	auto tFoundIt = audioImplemenet->m_Channels.find(channelId);
	if (tFoundIt == audioImplemenet->m_Channels.end())
		return;

	Sound::ErrorCheck(tFoundIt->second->setVolume(dbToVolume(volumedB)));
}

void Sound::LoadBank(const std::string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) 
{
	auto tFoundIt = audioImplemenet->m_Banks.find(bankName);
	if (tFoundIt != audioImplemenet->m_Banks.end())
		return;
	FMOD::Studio::Bank* pBank;
	Sound::ErrorCheck(audioImplemenet->m_soundSystem->loadBankFile(bankName.c_str(), flags, &pBank));
	if (pBank) 
	{
		audioImplemenet->m_Banks[bankName] = pBank;
	}
}

void Sound::LoadEvent(const std::string& eventName)
{
	auto tFoundit = audioImplemenet->m_Events.find(eventName);
	if (tFoundit != audioImplemenet->m_Events.end())
		return;
	FMOD::Studio::EventDescription* pEventDescription = NULL;
	Sound::ErrorCheck(audioImplemenet->m_soundSystem->getEvent(eventName.c_str(), &pEventDescription));
	if (pEventDescription) 
	{
		FMOD::Studio::EventInstance* pEventInstance = NULL;
		Sound::ErrorCheck(pEventDescription->createInstance(&pEventInstance));
		if (pEventInstance) 
		{
			audioImplemenet->m_Events[eventName] = pEventInstance;
		}
	}
}

void Sound::PlayEvent(const string &eventName) 
{
	auto tFoundit = audioImplemenet->m_Events.find(eventName);
	if (tFoundit == audioImplemenet->m_Events.end()) 
	{
		LoadEvent(eventName);
		tFoundit = audioImplemenet->m_Events.find(eventName);
		if (tFoundit == audioImplemenet->m_Events.end())
			return;
	}
	tFoundit->second->start();
}

void Sound::StopEvent(const string &eventName, bool bImmediate) 
{
	auto tFoundIt = audioImplemenet->m_Events.find(eventName);
	if (tFoundIt == audioImplemenet->m_Events.end())
		return;
	FMOD_STUDIO_STOP_MODE eMode;
	eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
	Sound::ErrorCheck(tFoundIt->second->stop(eMode));
}

bool Sound::IsEventPlaying(const string &eventName) const 
{
	auto tFoundIt = audioImplemenet->m_Events.find(eventName);
	if (tFoundIt == audioImplemenet->m_Events.end())
		return false;

	FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
	if (tFoundIt->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) {
		return true;
	}
	return false;
}

void Sound::GetEventParameter(const string &eventName, const string &strParameterName, float* parameter) 
{
	auto tFoundIt = audioImplemenet->m_Events.find(eventName);
	if (tFoundIt == audioImplemenet->m_Events.end())
		return;
	FMOD::Studio::ParameterInstance* pParameter = NULL;
	Sound::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	Sound::ErrorCheck(pParameter->getValue(parameter));
}

void Sound::SetEventParameter(const string &eventName, const string &strParameterName, float fValue) 
{
	auto tFoundIt = audioImplemenet->m_Events.find(eventName);
	if (tFoundIt == audioImplemenet->m_Events.end())
		return;
	FMOD::Studio::ParameterInstance* pParameter = NULL;
	Sound::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	Sound::ErrorCheck(pParameter->setValue(fValue));
}

FMOD_VECTOR Sound::VectorToFmod(const Vector3& vPosition) 
{
	FMOD_VECTOR fVec;
	fVec.x = vPosition.x;
	fVec.y = vPosition.y;
	fVec.z = vPosition.z;
	return fVec;
}

int Sound::ErrorCheck(FMOD_RESULT result) 
{
	if (result != FMOD_OK)
	{
		cout << "FMOD ERROR " << result << endl;
		return 1;
	}
	// cout << "FMOD all good" << endl;
	return 0;
}

float  Sound::dbToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float  Sound::VolumeTodB(float volume)
{
	return 20.0f * log10f(volume);
}

void Sound::Shutdown() 
{
	delete audioImplemenet;
}