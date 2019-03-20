#pragma once

#include "BusNode.h"

// 

class Entity : public BusNode
{
public:
	Entity();
	Entity(MessageBus* messageBus) : BusNode(messageBus) {}
	~Entity();

	void update();

private:
	void onNotify(Message message);
};

