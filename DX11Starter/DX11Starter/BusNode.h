#pragma once

#include <iostream>
#include <functional>
#include "MessageBus.h"

class BusNode
{
public:
	BusNode();
	BusNode(MessageBus * messageBus);
	~BusNode();

	virtual void update();

protected:
	MessageBus * messageBus;

	std::function<void(Message)> getNotifyFunc();

	void send(Message message);

	virtual void onNotify(Message message);
};

