#include "RectangleRenderer.h"

RectangleRenderer::RectangleRenderer()
{
	Drawable = &Shape;
}

RectangleRenderer::~RectangleRenderer()
{
}

void RectangleRenderer::Start()
{
}

void RectangleRenderer::Update(float deltaTime)
{
}

void RectangleRenderer::Destroy()
{
}

void RectangleRenderer::SetSize(const sf::Vector2f& size)
{
	Shape.setSize(size);
}

void RectangleRenderer::SetFillColor(const sf::Color& color)
{
	Shape.setFillColor(color);
}
