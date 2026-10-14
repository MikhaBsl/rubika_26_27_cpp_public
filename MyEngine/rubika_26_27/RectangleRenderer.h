#pragma once
#include <IDrawable.h>
#include <SFML/Graphics/RectangleShape.hpp>

class RectangleRenderer : public IDrawable
{
public:
	sf::RectangleShape Shape;

	RectangleRenderer();
	~RectangleRenderer();

	// Hérité via IDrawable
	void Start() override;
	void Update(float deltaTime) override;
	void Destroy(float deltaTime) override;

	void SetSize();
	void SetFillColor();
};