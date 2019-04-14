#pragma once

#include <Windows.h>
#include "System.h"
#include <gainput/gainput.h>

enum Button
{
	ButtonLeft,
	ButtonRight
};

class Input : public System {
public:
	Input() {}
	Input(EventBus * eventBusPtr, unsigned int width, unsigned int height) : System(eventBusPtr), screenWidth(width), screenHeight(height) {}
	~Input() {}

	void Init();
	void GetInput();

private:
	unsigned int screenWidth;
	unsigned int screenHeight;

	gainput::InputManager manager;
	
	gainput::DeviceId keyboardId;
	gainput::DeviceId mouseId;

	bool cameraControlMode = false;
};

