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
		InputEvent * ie = DBG_NEW InputEvent("Moving Left");
		// eventBus->publish(DBG_NEW InputEvent("Moving Left"));
		eventBus->publish(ie);
		delete ie;
	}
	if (GetKeyState('D') & 0x800)
	{
		InputEvent * ie = DBG_NEW InputEvent("Moving Right");
		// eventBus->publish(DBG_NEW InputEvent("Moving Right"));
		eventBus->publish(ie);
		delete ie;
	}
}
