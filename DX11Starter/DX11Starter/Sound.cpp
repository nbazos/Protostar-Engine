#include "Sound.h"

// Attritbutes
bool Sound::m_isPlaying = true;
bool Sound::m_isReady = true;
char* Sound::m_currentSound;

FMOD_RESULT Sound::m_result;
FMOD_SYSTEM* Sound::m_soundSystem;
FMOD_SYSTEM* Sound::m_soundSystemEffect;
FMOD_SOUND* Sound::m_backgroud;
FMOD_SOUND* Sound::m_effect;
FMOD_CHANNEL* Sound::m_channel;
FMOD_CHANNEL* Sound::m_channelEffect;
FMOD_VECTOR listenerVelocity, listenerUp, listenerForward, listenerPos;
FMOD_VECTOR* m_position;
FMOD_VECTOR* m_altPanPos;

Sound::~Sound()
{

}

// --------------------------------------------------------
// Initializes the audio engine system(s)
// --------------------------------------------------------
void Sound::Init()
{
	// Subscribes to the event(s) that the Sound system will respond to
	eventBus->Subscribe(this, &Sound::TogglePause);
	eventBus->Subscribe(this, &Sound::ChangeBackground);
	eventBus->Subscribe(this, &Sound::SwordSlash);

	m_result = FMOD_System_Create(&m_soundSystem);
	m_result = FMOD_System_Create(&m_soundSystemEffect);

	// Checks if there were no errors with creating the sound system
	if (m_result != FMOD_OK)
	{
		m_isReady = false;
		cout << "ERROR " << m_result << " " << FMOD_ErrorString(m_result);
	}

	if (m_isReady == true)
	{
		// Standard Sound System
		m_result = FMOD_System_Init(m_soundSystem, 10, FMOD_INIT_3D_RIGHTHANDED, 0);
		FMOD_System_Set3DSettings(m_soundSystem, DOPPLER_SCALE, DISTANCE_FACTOR, ROLLOFF_SCALE);

		// Sound Effect System
		m_result = FMOD_System_Init(m_soundSystemEffect, 10, FMOD_INIT_3D_RIGHTHANDED, 0);
		FMOD_System_Set3DSettings(m_soundSystemEffect, DOPPLER_SCALE, DISTANCE_FACTOR, ROLLOFF_SCALE);

		//cout << "FMOD CHECK 1 - Initalizing..." << endl;
	}

	// Checks if there were no errors with initalizing the system with
	// the proper channels
	if (m_result != FMOD_OK)
	{
		m_isReady = false;
		cout << "ERROR " << m_result << " " << FMOD_ErrorString(m_result);
	}

	// Sets the volume for the channel
	if (m_isReady == true)
	{
		FMOD_Channel_SetVolume(m_channel, 1.0f);
		FMOD_Channel_SetVolume(m_channelEffect, 1.0f);
		//cout << "FMOD CHECK 2 - Setting Volume..." << endl;
	}

	// Plays the background initally, which will upload before starting
	LoadFile(m_soundSystem, m_backgroud, "../../DX11Starter/audio/psycho.wav");
	Play(m_soundSystem, m_channel, m_backgroud, m_position, m_altPanPos);
}

//TODO: 3D Positioning
void Sound::UpdateListener()
{
	// Used to update the listener's position for more 3D aspects
}

void Sound::UpdateSound()
{
	// Used to update sound position based off game
}

// --------------------------------------------------------
// Update the system according to the listener
// --------------------------------------------------------
void Sound::UpdateSystem(FMOD_SYSTEM* audioSystem)
{
	FMOD_System_Set3DListenerAttributes(audioSystem, 0, &listenerPos, &listenerVelocity, &listenerForward, &listenerUp);
	FMOD_System_Update(audioSystem);
}

// --------------------------------------------------------
// Sets the volume between 0.0f and 1.0f
// --------------------------------------------------------
void Sound::SetVolume(FMOD_CHANNEL* channel, float volume)
{
	if (m_isReady && m_isPlaying && volume >= 0.0f && volume <= 1.0f)
	{
		FMOD_Channel_SetVolume(channel, volume);
	}
}

// --------------------------------------------------------
// Loads the selected file from the appropriate directory
// --------------------------------------------------------
void Sound::LoadFile(FMOD_SYSTEM* audioSystem, FMOD_SOUND* sound, const char * file)
{
	m_currentSound = (char*)file;

	if (m_isReady && m_isPlaying == true)
	{
		m_result = FMOD_Sound_Release(sound);
		m_result = FMOD_System_CreateStream(audioSystem, m_currentSound, FMOD_3D, 0, &sound);
		//cout << "FMOD CHECK 3 - Loading File..." << endl;

		// If file is not found an error is displayed
		if (m_result != FMOD_OK)
		{
			cout << "FMOD ERROR " << m_result << " " << FMOD_ErrorString(m_result);
			m_isReady = false;
		}
	}

	// Sets a channels priority. Higher priority means it is less likely to get discarded when
	// FSOUND_FREE is used to select a channel, when all channels are being used, and one has to
	// be rejected.If a channel has an equal priority then it will be replaced.
	// FMOD_Channel_SetPriority(m_channel, 255);
}

// --------------------------------------------------------
// Unloads the recent loaded file
// --------------------------------------------------------
void Sound::UnloadFile(FMOD_SOUND* sound)
{
	if (m_isReady == true)
	{
		m_result = FMOD_Sound_Release(sound);
	}
}

// --------------------------------------------------------
// Plays the loaded audio file
// --------------------------------------------------------
void Sound::Play(FMOD_SYSTEM* audioSystem, FMOD_CHANNEL* channel, FMOD_SOUND* sound, FMOD_VECTOR* position, FMOD_VECTOR* altPanPos)
{
	if (m_isReady && m_isPlaying == true)
	{
		m_result = FMOD_Channel_Set3DAttributes(channel, position, 0, altPanPos);
		m_result = FMOD_System_PlaySound(audioSystem, sound, FMOD_DEFAULT, false, &channel);
		FMOD_Channel_SetMode(channel, FMOD_LOOP_NORMAL);
		//cout << "FMOD CHECK 4 - Playing Background" << endl;
	}
}

// --------------------------------------------------------
// Plays the loaded sound effect event
// --------------------------------------------------------
void Sound::SwordSlash(SwordSlashes * soundEvent)
{
	LoadFile(m_soundSystemEffect, m_effect, "../../DX11Starter/audio/soundEffects/bulletFire.wav");
	Play(m_soundSystemEffect, m_channelEffect, m_effect, m_position, m_altPanPos);
}

// --------------------------------------------------------
// Changes background audio based on the scene
// --------------------------------------------------------
void Sound::ChangeBackground(SceneChange * soundEvent)
{
	// Wanted to create if statements depending on the scene and load
	// the proper audio file accordingly
	LoadFile(m_soundSystem, m_backgroud, "../../DX11Starter/audio/backgrnd.wav");
	Play(m_soundSystem, m_channel, m_backgroud, m_position, m_altPanPos);
}

// --------------------------------------------------------
// Checks if audio is paused
// --------------------------------------------------------
void Sound::GetPause(FMOD_BOOL p)
{
	FMOD_Channel_GetPaused(m_channel, &p);
}

// --------------------------------------------------------
// Checks if audio is playing
// --------------------------------------------------------
bool Sound::GetSound()
{
	return m_isPlaying;
}

// --------------------------------------------------------
// Sets audio pause to true or false
// --------------------------------------------------------
void Sound::SetPause(FMOD_BOOL p)
{
	FMOD_Channel_SetPaused(m_channel, !p);
}

// --------------------------------------------------------
// Sets sound to true or false
// --------------------------------------------------------
void Sound::SetSound(bool sound)
{
	m_isPlaying = sound;
}

// --------------------------------------------------------
// Loads the current sound or unload the file
// --------------------------------------------------------
void Sound::ToggleBackground()
{
	m_isPlaying = !m_isPlaying;

	if (m_isPlaying == true)
	{
		LoadFile(m_soundSystem, m_backgroud, m_currentSound);
	}

	if (m_isPlaying == false)
	{
		UnloadFile(m_backgroud);
	}
}

// --------------------------------------------------------
// Pauses or unpauses the current channel
// --------------------------------------------------------
void Sound::TogglePause(PauseAudio * soundEvent)
{
	FMOD_BOOL p;
	GetPause(p);
	SetPause(p);
	cout << "Audio Pause Status (0 - Pause | 1 - Play): " << p << endl;
}