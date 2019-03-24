#pragma once

#include <Windows.h>
#include "System.h"

class Input : public System {
public:
	Input();
	Input(EventBus * eventBusPtr) : System(eventBusPtr) {}
	~Input();

	void getInput();
};

