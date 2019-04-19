#pragma once

class Event {
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

struct InputMoveLeft		: public Event {};
struct InputMoveRight		: public Event {};
struct InputJump			: public Event {};
struct InputQuickAddEntity	: public Event {};