#pragma once

#define DOPPLER_SCALE         1.0
#define DISTANCE_FACTOR       1.0
#define ROLLOFF_SCALE         0.5

#include "System.h"
#include "inc\fmod.hpp"
#include "inc\fmod.h"
#include "inc\fmod_errors.h"
#include "inc\fmod_dsp.h"
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
	void UpdateListener();
	void UpdateSound();
	void UpdateSystem(FMOD_SYSTEM* audioSystem);

	void SetVolume(FMOD_CHANNEL* channel, float volume);
	void LoadFile(FMOD_SYSTEM* audioSystem, FMOD_SOUND* sound, const char* file);
	void UnloadFile(FMOD_SOUND* sound);
	void Play(FMOD_SYSTEM* audioSystem, FMOD_CHANNEL* channel, FMOD_SOUND* sound, FMOD_VECTOR* position, FMOD_VECTOR* altPanPos);
	void SwordSlash(SwordSlashes * soundEvent);
	void ChangeBackground(SceneChange * soundEvent);

	void GetPause(FMOD_BOOL p);
	bool GetSound();

	void SetPause(FMOD_BOOL p);
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