#pragma once
#include <IDrawable.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>

class RectangleRenderer : public IDrawable
{
public:
	sf::RectangleShape Shape;

	RectangleRenderer();
	~RectangleRenderer();

	// Hérité via IDrawable
	void Start() override;
	void Update(float deltaTime) override;
	void Destroy() override;

	void SetSize(const sf::Vector2f& size);
	void SetFillColor(const sf::Color& color);
};