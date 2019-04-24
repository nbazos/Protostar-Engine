#pragma once

#include <Windows.h>
#include "System.h"

class Input : public System 
{
public:
	Input();
	Input(EventBus * eventBusPtr) : System(eventBusPtr) {}
	~Input();

	void Init();
	void GetInput();
	void ProcessKeyDown(WPARAM keyCode);
	void ProcessKeyUp(WPARAM keyCode);

private:
	bool cameraControlMode = false;
	std::string vkToString(int vk);
};
