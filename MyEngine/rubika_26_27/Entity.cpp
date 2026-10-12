#include "Entity.h"

Entity::Entity(std::string friendlyName) 
{
	name = friendlyName;
	transformComponent = TransformComponent();
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
