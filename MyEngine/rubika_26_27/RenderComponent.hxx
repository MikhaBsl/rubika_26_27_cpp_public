#pragma once

#include <type_traits>
#include <memory>
#include <Logger.h>
#include "RenderComponent.h"

template <typename D>
D* RenderComponent::AddDrawable(const std::string& friendlyName)
{
	static_assert(std::is_base_of_v<IDrawable, D>, "D must inherit from IDrawable");

	for (const DrawableInfo& info : Drawables)
	{
		if (info.FriendlyName == friendlyName)
		{
			Logger::Warning("Drawable name already used : " + friendlyName);
			return nullptr;
		}
	}

	auto drawable = std::make_unique<D>();
	D* raw = drawable.get();

	DrawableInfo& info = Drawables.emplace_back();
	info.Drawable = std::move(drawable);
	info.FriendlyName = friendlyName;

	return raw;
}

template <typename D>
D* RenderComponent::AddDrawable(const std::string& friendlyName, sf::Vector2f relativePos, float relativeAngle, sf::Vector2f relativeScale)
{
	D* raw = AddDrawable<D>(friendlyName);
	if (raw == nullptr)
		return nullptr;

	DrawableInfo& info = Drawables.back();
	info.RelativePosition = relativePos;
	info.RelativeRotation = relativeAngle;
	info.RelativeScale = relativeScale;
	info.ComputeTransform();

	return raw;
}

template <typename D>
D* RenderComponent::GetDrawable() const
{
	static_assert(std::is_base_of_v<IDrawable, D>, "D must inherit from IDrawable");

	for (const DrawableInfo& info : Drawables)
	{
		if (D* result = dynamic_cast<D*>(info.Drawable.get()))
			return result;
	}
	return nullptr;
}