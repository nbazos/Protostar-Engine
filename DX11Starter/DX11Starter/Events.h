#pragma once

class Event
{
protected:
	virtual ~Event() {}
};

// --------------------------------------------------------
// Input Events
// --------------------------------------------------------

struct InputEvent : public Event {
	InputEvent(std::string str) : STR{ str } {}
	std::string STR;
	//~InputEvent() {}
};

struct InputMoveLeft : public Event {};
struct InputMoveRight : public Event {};
struct InputJump : public Event {};
struct InputQuickAddEntity : public Event {};

// Audio Playback Events
struct PauseAudio : public Event {};
struct PlayBackground : public Event {};
struct WalkingSteps : public Event {};
struct RunningSteps : public Event {};
struct SceneChange : public Event {};
struct SwordSlashes : public Event {};
struct HealthDecrease : public Event {};
struct HealthIncrease : public Event {};
struct PlayerDeath : public Event {};
struct EnemyDetection : public Event {};
