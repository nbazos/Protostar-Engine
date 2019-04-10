#pragma once

class Event 
{
protected:
	virtual ~Event() {}
};


// --------------------------------------------------------
// Input Events
// --------------------------------------------------------

struct InputEvent : public Event
{
	InputEvent(std::string str) : STR{ str } {}
	std::string STR;
	//~InputEvent() {}
};

struct InputMoveLeft : public Event {};
struct InputMoveRight : public Event {};
struct PauseAudio : public Event {};
struct PlayBackground : public Event {};
struct PlayBulletFire : public Event {};
struct SceneChange : public Event {};
