#include "Entity.h"
#include <RenderComponent.h>

Entity::Entity(std::string friendlyName)
{
	name = friendlyName;
}

Entity::~Entity()
{
}

void Entity::Start()
{
	for (auto& component : components)
		component->Start();
}

void Entity::Update(float fDeltaTime)
{
	for (auto& component : components)
		component->Update(fDeltaTime);
}

void Entity::Destroy()
{
	for (auto& component : components)
		component->Destroy();

	delete this;
}

void Entity::Draw(sf::RenderWindow& window) const
{
	RenderComponent* render = GetComponent<RenderComponent>();
	if (render != nullptr)
		render->Draw(window);
}
