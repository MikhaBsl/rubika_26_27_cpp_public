#include "Obstacle.h"
#include <TransformComponent.h>
#include <RenderComponent.h>
#include <DynamicColliderComponent.h>
#include <RectangleRenderer.h>

Obstacle::Obstacle( std::string fileName, std::string tileName, std::string friendlyName) : Entity(friendlyName)
{
	name = friendlyName;
	this->AddComponent<TransformComponent>();
	this->AddComponent<RenderComponent>();
	this->AddComponent<DynamicColliderComponent>();

	DrawObstacle(fileName, tileName);
}

Obstacle::~Obstacle()
{
}

void Obstacle::Start()
{
	Entity::Start();
}

void Obstacle::Update(float fDeltaTime)
{
	Entity::Update(fDeltaTime);
}

void Obstacle::Destroy()
{
	Entity::Destroy();
}

void Obstacle::DrawObstacle(std::string fileName, std::string tileName) const
{
	RenderComponent* render = GetComponent<RenderComponent>();

	RectangleRenderer* bloc = render->AddDrawable<RectangleRenderer>("Block", sf::Vector2f(0.f, 0.f), 0.f, sf::Vector2f(1.f, 1.f));
	bloc->SetTextureAndStaticTile(("{}", fileName), ("{}", tileName));

	DynamicColliderComponent* collider = this->GetComponent<DynamicColliderComponent>();
	sf::FloatRect bounds = render->GetLocalBounds();
	collider->SetHitboxSize(bounds.size);
	collider->SetHitboxOffset(bounds.position);
}
