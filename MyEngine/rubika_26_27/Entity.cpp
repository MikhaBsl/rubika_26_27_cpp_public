#include "Entity.h"

Entity::Entity(std::string friendlyName) : transformComponent(*this), renderComponent(*this)
{
	name = friendlyName;

	auto rect = std::make_unique<RectangleRenderer>();
	rect->SetSize(sf::Vector2f(100.f, 60.f));
	rect->SetFillColor(sf::Color::Red);

	sf::Transform transform;
	transform.translate(sf::Vector2f(200.f, 150.f));
	rect->SetWorldTransform(transform);

	renderComponent.SetDrawable(std::move(rect));
}

Entity::~Entity()
{
}

void Entity::Start()
{
	transformComponent.Start();
	renderComponent.Start();
}

void Entity::Update(float fDeltaTime)
{
	transformComponent.Update(fDeltaTime);
	renderComponent.Update(fDeltaTime);
}

void Entity::Destroy()
{
	delete this;
}

void Entity::Draw(sf::RenderWindow& window) const
{
	renderComponent.Draw(window);
}
