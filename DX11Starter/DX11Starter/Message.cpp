#include "Message.h"

Message::Message()
{
}

Message::Message(const std::string event)
{
	messageEvent = event;
}

Message::~Message()
{
}

std::string Message::getEvent()
{
	return messageEvent;
}
