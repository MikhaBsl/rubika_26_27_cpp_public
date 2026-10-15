#include "RenderComponent.h"
#include <IDrawable.h>

RenderComponent::RenderComponent(Entity& parent) : IComponent(parent)
{
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::Start()
{
	if (Drawable)
		Drawable->Start();
}

void RenderComponent::Update(float fDeltaTime)
{
	if (Drawable)
		Drawable->Update(fDeltaTime);
}

void RenderComponent::Destroy()
{
	if (Drawable)
		Drawable->Destroy();
}

void RenderComponent::SetDrawable(std::unique_ptr<IDrawable> drawable)
{
	Drawable = std::move(drawable);
}

void RenderComponent::Draw(sf::RenderWindow& window) const
{
	if (Drawable)
		Drawable->Draw(window);
}