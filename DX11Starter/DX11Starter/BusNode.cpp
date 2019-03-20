#include "BusNode.h"



BusNode::BusNode()
{
}

BusNode::BusNode(MessageBus * messageBus)
{
	this->messageBus = messageBus;
	this->messageBus->addReceiver(this->getNotifyFunc());
}


BusNode::~BusNode()
{
}

void BusNode::update()
{
}

std::function<void(Message)> BusNode::getNotifyFunc()
{
	auto messageListener = [=](Message message) -> void {
		this->onNotify(message);
	};
	return messageListener;
}

void BusNode::send(Message message)
{
	messageBus->sendMessage(message);
}

void BusNode::onNotify(Message message)
{
	// Do something here. Your choice. You could do something like this.
	// std::cout << "Siopao! Siopao! Siopao! (Someone forgot to implement onNotify().)" << std::endl;
}
