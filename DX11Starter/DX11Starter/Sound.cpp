#include "Sound.h"

#define DOPPLER_SCALE         1.0
#define DISTANCE_FACTOR       1.0
#define ROLLOFF_SCALE         0.5

// Attritbutes
bool Sound::m_isPlaying = true;
bool Sound::m_isReady = true;
char* Sound::m_currentSound;

FMOD_RESULT Sound::m_result;
FMOD_SYSTEM* Sound::m_soundSystem;
FMOD_SOUND* Sound::m_backgroud;
FMOD_CHANNEL* Sound::m_channel;
FMOD_VECTOR listenerVelocity, listenerUp, listenerForward, listenerPos;
FMOD_VECTOR* m_position;
FMOD_VECTOR* m_altPanPos;

Sound::~Sound()
{
	/*
	delete m_currentSound;
	delete m_soundSystem;
	delete m_backgroud;
	delete m_channel;
	*/
}

// Initializes the audio engine
void Sound::Init()
{
	// Subscribes to the event(s) that the Sound system will respond to
	eventBus->Subscribe(this, &Sound::TogglePause);

	m_result = FMOD_System_Create(&m_soundSystem);

	// Checks if there were no errors with creating the sound system
	if (m_result != FMOD_OK)
	{
		m_isReady = false;
		cout << "FMOD ERROR " << m_result << " " << FMOD_ErrorString(m_result);
	}

	if (m_isReady == true)
	{
		m_result = FMOD_System_Init(m_soundSystem, 10, FMOD_INIT_NORMAL, 0);
		cout << "FMOD CHECK 1 - Initalizing" << endl;
	}

	// Checks if there were no errors with initalizing the system with
	// the proper channels
	if (m_result != FMOD_OK)
	{
		m_isReady = false;
		cout << "FMOD ERROR " << m_result << " " << FMOD_ErrorString(m_result);
	}

	// Sets the volume for the channel
	if (m_isReady == true)
	{
		FMOD_Channel_SetVolume(m_channel, 1.0f);
		cout << "FMOD CHECK 2 - Setting Volume" << endl;
	}
}

// Initalize sound system in a 3D space
void Sound::Init3D()
{
	// Subscribes to the event(s) that the Sound system will respond to
	eventBus->Subscribe(this, &Sound::TogglePause);

	m_result = FMOD_System_Create(&m_soundSystem);

	// Checks if there were no errors with creating the sound system
	if (m_result != FMOD_OK)
	{
		m_isReady = false;
		cout << "FMOD ERROR " << m_result << " " << FMOD_ErrorString(m_result);
	}

	if (m_isReady == true)
	{
		m_result = FMOD_System_Init(m_soundSystem, 10, FMOD_INIT_3D_RIGHTHANDED, 0);
		FMOD_System_Set3DSettings(m_soundSystem, DOPPLER_SCALE, DISTANCE_FACTOR, ROLLOFF_SCALE);
		cout << "FMOD CHECK 1 - Initalizing 3D" << endl;
	}

	// Checks if there were no errors with initalizing the system with
	// the proper channels
	if (m_result != FMOD_OK)
	{
		m_isReady = false;
		cout << "FMOD ERROR " << m_result << " " << FMOD_ErrorString(m_result);
	}

	// Sets the volume for the channel
	if (m_isReady == true)
	{
		FMOD_Channel_SetVolume(m_channel, 1.0f);
		cout << "FMOD CHECK 2 - Setting Volume 3D" << endl;
	}

	Play3D();
}

void Sound::UpdateListener()
{
	// Used to update the listener's position
}

void Sound::UpdateSound()
{
	// Used to update sound position based off game
}

// Update the system according to where the listener is listening from
void Sound::UpdateSystem()
{
	FMOD_System_Set3DListenerAttributes(m_soundSystem, 0, &listenerPos, &listenerVelocity, &listenerForward, &listenerUp);
	FMOD_System_Update(m_soundSystem);
}

// Sets the volume between 0.0f and 1.0f
void Sound::SetVolume(float volume)
{
	if (m_isReady && m_isPlaying && volume >= 0.0f && volume <= 1.0f)
	{
		FMOD_Channel_SetVolume(m_channel, volume);
	}
}

// Loads the selected file from the appropriate directory
void Sound::LoadFile(const char * file)
{
	m_currentSound = (char*)file;

	if (m_isReady && m_isPlaying == true)
	{
		m_result = FMOD_Sound_Release(m_backgroud);
		m_result = FMOD_System_CreateStream(m_soundSystem, m_currentSound, FMOD_DEFAULT, 0, &m_backgroud);
		cout << "FMOD CHECK 3 - Loading File" << endl;

		// If file is not found an error is displayed
		if (m_result != FMOD_OK)
		{
			cout << "FMOD ERROR " << m_result << " " << FMOD_ErrorString(m_result);
			m_isReady = false;
		}
	}
}

void Sound::LoadFile3D(const char * file)
{
	m_currentSound = (char*)file;

	if (m_isReady && m_isPlaying == true)
	{
		m_result = FMOD_Sound_Release(m_backgroud);
		m_result = FMOD_System_CreateStream(m_soundSystem, m_currentSound, FMOD_3D, 0, &m_backgroud);
		cout << "FMOD CHECK 3 - Loading File 3D" << endl;

		// If file is not found an error is displayed
		if (m_result != FMOD_OK)
		{
			cout << "FMOD ERROR " << m_result << " " << FMOD_ErrorString(m_result);
			m_isReady = false;
		}
	}
}

// Unloads the recent loaded file
void Sound::UnloadFile()
{
	if (m_isReady == true)
	{
		m_result = FMOD_Sound_Release(m_backgroud);
	}
}

// Plays the loaded audio file
void Sound::Play()
{
	if (m_isReady && m_isPlaying == true)
	{
		m_result = FMOD_System_PlaySound(m_soundSystem, m_backgroud, FMOD_DEFAULT, false, &m_channel);
		FMOD_Channel_SetMode(m_channel, FMOD_LOOP_NORMAL);
		cout << "FMOD CHECK 4 - Playing Background" << endl;
	}
}

void Sound::Play3D()
{
	LoadFile3D("../../DX11Starter/audio/psycho.wav");

	if (m_isReady && m_isPlaying == true)
	{
		m_result = FMOD_Channel_Set3DAttributes(m_channel, m_position, 0, m_altPanPos);
		m_result = FMOD_System_PlaySound(m_soundSystem, m_backgroud, FMOD_DEFAULT, false, &m_channel);
		FMOD_Channel_SetMode(m_channel, FMOD_LOOP_NORMAL);
		cout << "FMOD CHECK 4 - Playing Background 3D" << endl;
	}
}

void Sound::Bullet(PlayBulletFire * soundEvent)
{
	LoadFile3D("../../DX11Starter/audio/soundEffects/bulletFire.wav");

	if (m_isReady && m_isPlaying == true)
	{
		m_result = FMOD_Channel_Set3DAttributes(m_channel, m_position, 0, m_altPanPos);
		m_result = FMOD_System_PlaySound(m_soundSystem, m_backgroud, FMOD_DEFAULT, false, &m_channel);
		FMOD_Channel_SetMode(m_channel, FMOD_LOOP_NORMAL);
		cout << "Firing Bullet" << endl;
	}
}

// Returns the current played audio
bool Sound::GetSound()
{
	return m_isPlaying;
}

// Sets audio pause to true or false
void Sound::SetPause(bool pause)
{
	FMOD_Channel_SetPaused(m_channel, pause);
}

// Sets sound to true or false
void Sound::SetSound(bool sound)
{
	m_isPlaying = sound;
}

// Loads the current sound and play it or unload the file
void Sound::ToggleBackground()
{
	m_isPlaying = !m_isPlaying;

	if (m_isPlaying == true)
	{
		LoadFile(m_currentSound);
		//Play3D();
	}

	if (m_isPlaying == false)
	{
		UnloadFile();
	}
}

// Pauses or unpauses the current channel
void Sound::TogglePause(PauseAudio * soundEvent)
{
	FMOD_BOOL h;
	FMOD_Channel_GetPaused(m_channel, &h);
	FMOD_Channel_SetPaused(m_channel, !h);
	cout << "Audio Pause Status: " << h << endl;
}