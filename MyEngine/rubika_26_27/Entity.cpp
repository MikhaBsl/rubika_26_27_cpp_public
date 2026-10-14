#include "Entity.h"

Entity::Entity(std::string friendlyName) : transformComponent(*this), renderComponent()
{
	name = friendlyName;
}

Entity::~Entity()
{
}

void Entity::Start()
{
}

void Entity::Update(float fDeltaTime)
{
}

void Entity::Destroy()
{
	delete this;
}
