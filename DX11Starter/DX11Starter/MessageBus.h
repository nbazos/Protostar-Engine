#pragma once
#include "Message.h"
#include <functional>
#include <queue>
#include <vector>

class MessageBus
{
public:
	MessageBus();
	~MessageBus();

	void addReceiver(std::function<void(Message)> messageReceiver);

	void sendMessage(Message message);

	void notify();

private:
	std::vector<std::function<void(Message)>> receivers;
	std::queue<Message> messages;
};

