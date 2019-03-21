#pragma once

#include "EventBus.h"

class System
{
public:
	System() {}
	System(EventBus * eventBusPtr)
	{
		eventBus = eventBusPtr;
	}
	~System() {}

	virtual void init() {}
protected:
	EventBus * eventBus;
};

