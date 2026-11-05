#pragma once

#include <IComponent.h>

#include <SfML/Graphics/Rect.hpp>
#include <functional>
#include <TransformComponent.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics.hpp>

enum class OnCollisionChoice
{
	Ignore,
	Block,
};

class DynamicColliderComponent : public IComponent
{
public:
	DynamicColliderComponent(Entity& entity);
	~DynamicColliderComponent();

	virtual void Start() override;
	virtual void Update(float fDeltaTime) override;
	virtual void Destroy() override;

	void SetHitboxSize(sf::Vector2f size);
	void SetHitboxOffset(sf::Vector2f offset);

	void EnableCollision(bool bEnable);
	bool AreCollisionsEnabled() const;

	const sf::FloatRect& GetHitbox() const;

	void SetOnCollisionCallback(std::function<OnCollisionChoice(const Entity*)> callback);

	const char* GetName() const override;
	void DrawDebug() override;

	void Draw() const;

private: 
	sf::FloatRect Hitbox;
	sf::Vector2f HitboxOffset;
	bool bCollisionEnabled = true;
	std::function<OnCollisionChoice(const Entity*)> OnCollisionCallback;

	TransformComponent* Transform = nullptr;
	void UpdateHitboxPosition();

	sf::RectangleShape RectDebug;
};