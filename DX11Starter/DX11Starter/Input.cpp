#include "Input.h"

void Input::Init()
{
	manager.SetDisplaySize(screenWidth, screenHeight);

	keyboardId = manager.CreateDevice<gainput::InputDeviceKeyboard>();
	mouseId = manager.CreateDevice<gainput::InputDeviceMouse>();
}

void Input::GetInput()
{
	gainput::InputMap map(manager);

	if (GetKeyState(VK_F3))
	{
		cameraControlMode = !cameraControlMode;
		std::cout << cameraControlMode << std::endl;
	}

	if (GetKeyState('A') & 0x800 && !cameraControlMode)
	{
		InputMoveLeft * ie = DBG_NEW InputMoveLeft();
		eventBus->Publish(ie);
		delete ie;
	}
	if (GetKeyState('D') & 0x800)
	{
		InputMoveRight * ie = DBG_NEW InputMoveRight();
		eventBus->Publish(ie);
		delete ie;
	}
}
