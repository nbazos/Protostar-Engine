#include "Input.h"


Input::Input()
{
}

Input::~Input()
{
}

void Input::update()
{
	Message greeting("Hi!");
	send(greeting);
}
