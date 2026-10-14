#pragma once
#include <IDrawable.h>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
class SpriteRenderer : public IDrawable
{
public:
	std::unique_ptr<sf::Sprite> Sprite;

	SpriteRenderer();
	~SpriteRenderer();

	// Hérité via IDrawable
	void Start() override;
	void Update(float deltaTime) override;
	void Destroy(float deltaTime) override;

	void SetTexture();
	void SetTextureRect();
};