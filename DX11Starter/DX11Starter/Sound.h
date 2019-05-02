#pragma once

// --------------------------------------------------------
// Prevents multiple definitions of objects in the header
// --------------------------------------------------------
#ifndef _SOUND_ENGINE_H
#define _SOUND_ENGINE_H

#define DOPPLER_SCALE         1.0
#define DISTANCE_FACTOR       1.0
#define ROLLOFF_SCALE         0.5

#include "System.h"
#include "inc/fmod.hpp"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;

// --------------------------------------------------------
// Used to place sound in the 3D space
// --------------------------------------------------------
struct Vector3
{
	float x;
	float y;
	float z;
};

// --------------------------------------------------------
// Contains the code for initializing and shutting down
// the sound engine; SoundImplement also holds a map of all
// the sounds played in our project
// --------------------------------------------------------
struct SoundImplement
{
	SoundImplement();
	~SoundImplement();

	void Update();

	FMOD::System* m_soundSystem;
	FMOD::System* m_system;

	int m_nextChannelId;

	typedef map<string, FMOD::Sound*> SoundMap;
	typedef map<int, FMOD::Channel*> ChannelMap;

	SoundMap m_Sounds;
	ChannelMap m_Channels;
};

// --------------------------------------------------------
// Necessary methods for proper sound engine
// --------------------------------------------------------
class Sound : public System
{
public:
	Sound() {};
	Sound(EventBus * eventBusPtr) : System(eventBusPtr) {};
	~Sound() {};

	static void Init();
	static void Update();
	static void Shutdown();
	static int ErrorCheck(FMOD_RESULT result);

	void LoadEvent(const string& eventName);
	void LoadSound(const string & soundName, bool b_3d = true, bool b_Looping = false, bool b_Stream = false);
	void UnloadSound(const string & soundName);
	void Set3dListenerAndOrientation(const Vector3& pos, const Vector3& velocity);
	int PlaySounds(const string& soundName, const Vector3& pos = Vector3{ 0, 0, 0 }, float volumedB = 0.0f);
	void PlayEvent(const string& eventName);
	void StopChannel(int channelId);
	void StopEvent(const string& eventName, bool bImmediate = false);
	void GetEventParameter(const string& eventName, const string& eventParameter, float* parameter);
	void SetEventParameter(const string& eventName, const string& parameterName, float value);
	void StopAllChannels();
	void SetChannel3dPosition(int channelId, const Vector3& pos);
	void SetChannelVolume(int channelId, float volumedB);
	bool IsPlaying(int channelId) const;
	bool IsEventPlaying(const string& eventName) const;
	float dbToVolume(float dB);
	float VolumeTodB(float volume);
	FMOD_VECTOR VectorToFmod(const Vector3& pos);
};

#endif