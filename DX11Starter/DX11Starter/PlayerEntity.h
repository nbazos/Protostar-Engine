#pragma once

#include "System.h"
#include <iostream>

class PlayerEntity : public System {
public:
	PlayerEntity();
	PlayerEntity(EventBus * eventBusPtr) : System(eventBusPtr) {}
	~PlayerEntity();

	void init();

	void print(InputEvent * ie);
};

