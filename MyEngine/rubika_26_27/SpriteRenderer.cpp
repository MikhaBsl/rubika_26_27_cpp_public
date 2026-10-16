#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer()
{
	Drawable = Sprite.get();
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Start()
{
}

void SpriteRenderer::Update(float deltaTime)
{
}

void SpriteRenderer::Destroy()
{
}

void SpriteRenderer::SetTexture(const sf::Texture& texture)
{
	if (!Sprite)
		Sprite = std::make_unique<sf::Sprite>(texture);
	else
		Sprite->setTexture(texture, true);

	Drawable = Sprite.get();
}

void SpriteRenderer::SetTextureRect(const sf::IntRect& rect)
{
	if (Sprite)
		Sprite->setTextureRect(rect);
}

sf::Vector2f SpriteRenderer::GetSize() const
{
	if (!Sprite)
		return sf::Vector2f(0.f, 0.f);

	return Sprite->getGlobalBounds().size;
}
