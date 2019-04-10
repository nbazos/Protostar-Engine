#pragma once

#include "System.h"
#include "inc\fmod.hpp"
#include "inc\fmod.h"
#include "inc\fmod_errors.h"
#include "inc\fmod_dsp.h"
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
	void Init3D();
	void UpdateListener();
	void UpdateSound();
	void UpdateSystem();

	void SetVolume(float volume);
	void LoadFile(const char* file);
	void LoadFile3D(const char* file);
	void UnloadFile();
	void Play();
	void Play3D();
	void ChangeBackground(SceneChange * soundEvent, const char* file);
	void Bullet(PlayBulletFire * soundEvent);

	bool GetSound();

	void SetPause(bool pause);
	void SetSound(bool sound);

	void ToggleBackground();
	void TogglePause(PauseAudio * soundEvent);

private:
	static FMOD_SYSTEM* m_soundSystem;
	static FMOD_SOUND* m_backgroud;
	static FMOD_CHANNEL* m_channel;
	static FMOD_RESULT m_result;
	FMOD_VECTOR listenerVelocity, listenerUp, listenerForward, listenerPos;
	FMOD_VECTOR* m_position;
	FMOD_VECTOR* m_altPanPos;

	static bool m_isPlaying;
	static bool m_isReady;
	static char * m_currentSound;
};