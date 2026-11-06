#include "Player.h"
#include <TransformComponent.h>
#include <RenderComponent.h>
#include <Globals.h>
#include <SpriteRenderer.h>

Player::Player(std::string friendlyName) : Entity(friendlyName)
{
	name = friendlyName;
	this->AddComponent<TransformComponent>();
	this->AddComponent<RenderComponent>();
	this->AddComponent<DynamicColliderComponent>();

	this->GetComponent<DynamicColliderComponent>()->SetOnCollisionCallback([this](const Entity* other) 
		{ return this->OnCollision(other); });

	controller = new PlayerController(this->GetComponent<TransformComponent>());
	controller->Init();
	DrawPlayer();
}

Player::~Player()
{
	delete controller;
}

void Player::Start()
{
	Entity::Start();
}

void Player::Update(float fDeltaTime)
{
	Entity::Update(fDeltaTime);
	controller->Update(fDeltaTime);
}

void Player::Destroy()
{
	Entity::Destroy();
	controller->Destroy();
}

void Player::DrawPlayer() const
{
	RenderComponent* render = GetComponent<RenderComponent>();

	SpriteRenderer* body = render->AddDrawable<SpriteRenderer>("Body", sf::Vector2f(2.f, 0.f), 0.f, sf::Vector2f(1.f, 1.f));
	body->SetTextureAndAnimation("IsaacSprite", "Body_Vertical");

	SpriteRenderer* head = render->AddDrawable<SpriteRenderer>("Head", sf::Vector2f(-2.5f, -20.f), 0.f, sf::Vector2f(1.f, 1.f));
	head->SetTextureAndAnimation("IsaacSprite", "Head_Down");

	DynamicColliderComponent* collider = this->GetComponent<DynamicColliderComponent>();
	sf::FloatRect bounds = render->GetLocalBounds();
	collider->SetHitboxSize(bounds.size);
	collider->SetHitboxOffset(bounds.position);
}

OnCollisionChoice Player::OnCollision(const Entity* other) const
{
	return OnCollisionChoice::Block;
}
