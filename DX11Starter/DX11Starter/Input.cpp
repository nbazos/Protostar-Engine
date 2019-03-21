#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::getInput()
{
	if (GetKeyState('A') & 0x800)
	{
		eventBus->publish(new InputEvent("Moving Left"));
	}
	if (GetKeyState('D') & 0x800)
	{
		eventBus->publish(new InputEvent("Moving Right"));
	}
}
