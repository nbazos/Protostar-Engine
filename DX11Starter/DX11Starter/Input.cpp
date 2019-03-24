#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::getInput()
{
	if (GetAsyncKeyState('A') & 0x800)
	{
		eventBus->publish(new InputEvent("Moving Left"));
	}
	if (GetAsyncKeyState('D') & 0x800)
	{
		eventBus->publish(new InputEvent("Moving Right"));
	}
}
