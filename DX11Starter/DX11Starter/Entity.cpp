#include "Entity.h"
#include <string>


Entity::Entity()
{
}


Entity::~Entity()
{
}

void Entity::update()
{
	Message greeting("Hi!");
	send(greeting);
}

void Entity::onNotify(Message message)
{
	std::cout << "Entity received: " << message.getEvent() << std::endl;
}
