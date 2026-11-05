#include "Player.h"
#include <TransformComponent.h>
#include <RenderComponent.h>
#include <DynamicColliderComponent.h>
#include <Globals.h>
#include <SpriteRenderer.h>

Player::Player(std::string friendlyName)
{
	name = friendlyName;
	this->AddComponent<TransformComponent>();
	this->AddComponent<RenderComponent>();
	this->AddComponent<DynamicColliderComponent>();
	controller = new PlayerController(this->GetComponent<TransformComponent>());
	controller->Init();
	DrawPlayer();
}

Player::~Player()
{
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
}

void Player::DrawPlayer() const 
{
	RenderComponent* render = GetComponent<RenderComponent>();
	SpriteRenderer* spriteRenderer = render->AddDrawable<SpriteRenderer>("Sprite Player");

	const TextureData& playerTexture = gData.TextureMgr->GetTextureData("IsaacSprite");


	spriteRenderer->SetTexture("IsaacSprite");
	

}
