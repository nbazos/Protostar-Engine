#pragma once

#include "EventBus.h"

class System {
public:
	System() {}
	System(EventBus * eventBusPtr)
	{
		eventBus = eventBusPtr;
	}
	~System() {}

	virtual void Init() {}
protected:
	EventBus * eventBus;
};

