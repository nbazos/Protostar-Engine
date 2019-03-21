#include "PlayerEntity.h"

PlayerEntity::PlayerEntity()
{
}

PlayerEntity::~PlayerEntity()
{
}

void PlayerEntity::init()
{
	eventBus->subscribe(this, &PlayerEntity::print);
}

void PlayerEntity::print(InputEvent * ie)
{
	std::cout << ie->STR << std::endl;
}
