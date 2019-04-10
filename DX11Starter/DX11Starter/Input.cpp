#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::Init()
{
}

void Input::GetInput()
{
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
	if (GetKeyState('P') & 0x800) 
	{
		PauseAudio * ie = DBG_NEW PauseAudio();
		eventBus->Publish(ie);
		delete ie;
	}
	// Would get mouse left click and play bullet fire
	/*if (GetKeyState(MOUSEEVENTF_LEFTDOWN) & 0x800)
	{
		PlayBulletFire * ie = DBG_NEW PlayBulletFire();
		eventBus->Publish(ie);
		delete ie;
	}*/
}
