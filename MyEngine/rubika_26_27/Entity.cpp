#include "Entity.h"

Entity::Entity(std::string friendlyName) : transformComponent(*this), renderComponent(*this)
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
