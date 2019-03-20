#pragma once

#include "BusNode.h"

// Send a message to gameplay

class Input : public BusNode
{
public:
	Input();
	Input(MessageBus* messageBus):BusNode(messageBus) {}
	~Input();

	void update();
};

