#pragma once

#include "System.h"
#include "inc\fmod.hpp"
#include "inc\fmod.h"
#include "inc\fmod_errors.h"
#include "inc\fmod_dsp.h"
#include "SimpleMath.h"
#include <iostream>
#include <string>

using namespace std;

// --------------------------------------------------------
// Necessary methods for proper sound engine
// --------------------------------------------------------
class Sound : public System
{
public:
	Sound() {};
	Sound(EventBus * eventBusPtr) : System(eventBusPtr) {};
	~Sound();

	void Init();
	void InitSE();
	void UpdateListener();
	void UpdateSound();
	void UpdateSystem();

	void SetVolume(float volume);
	void LoadFile(const char* file);
	void LoadFileSE(const char* file);
	void UnloadFile();
	void Play();
	void PlaySE(PlayBulletFire * soundEvent);
	void ChangeBackground(SceneChange * soundEvent);

	bool GetSound();

	void SetPause(bool pause);
	void SetSound(bool sound);

	void ToggleBackground();
	void TogglePause(PauseAudio * soundEvent);

private:
	static FMOD_SYSTEM* m_soundSystem;
	static FMOD_SYSTEM* m_soundSystemEffect;
	static FMOD_SOUND* m_backgroud;
	static FMOD_SOUND* m_effect;
	static FMOD_CHANNEL* m_channel;
	static FMOD_CHANNEL* m_channelEffect;
	static FMOD_RESULT m_result;
	FMOD_VECTOR listenerVelocity, listenerUp, listenerForward, listenerPos;
	FMOD_VECTOR* m_position;
	FMOD_VECTOR* m_altPanPos;

	static bool m_isPlaying;
	static bool m_isReady;
	static char * m_currentSound;
	float cameraPosition;
};