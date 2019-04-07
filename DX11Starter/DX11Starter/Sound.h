#pragma once

#include "System.h"
#include "inc\fmod.hpp"
#include "inc\fmod_errors.h"
#include "inc\fmod.h"
#include <iostream>
#include <string>

using namespace std;

class Sound : public System 
{
public:
	Sound() {};
	Sound(EventBus * eventBusPtr) : System(eventBusPtr) {};
	~Sound();

	void Init();

	void SetVolume(float volume);
	void LoadFile(const char* file);
	void UnloadFile();
	void Play();

	bool GetSound();

	void SetPause(bool pause);
	void SetSound(bool sound);

	void ToggleBackground();
	void TogglePause(PlayAudioFile * inputEvent);

private:
	static FMOD_SYSTEM* m_soundSystem;
	static FMOD_SOUND* m_backgroud;
	static FMOD_CHANNEL* m_channel;
	static FMOD_RESULT m_result;

	static bool m_isPlaying;
	static bool m_isReady;
	static char * m_currentSound;
};
