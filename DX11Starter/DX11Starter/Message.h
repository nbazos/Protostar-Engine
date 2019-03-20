#pragma once

#include "BusNode.h"
#include <iostream>

class Message
{
public:
	Message();
	Message(const std::string event);
	~Message();

	std::string getEvent();

private:
	std::string messageEvent;
};

