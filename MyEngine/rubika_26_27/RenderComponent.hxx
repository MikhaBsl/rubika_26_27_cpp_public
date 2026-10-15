#pragma once

#include <type_traits>
#include <memory>
#include "RenderComponent.h"

template <typename D>
D* RenderComponent::AddDrawable()
{
	static_assert(std::is_base_of_v<IDrawable, D>, "D must inherit from IDrawable");

	auto drawable = std::make_unique<D>();
	D* raw = drawable.get();
	Drawables.push_back(std::move(drawable));
	return raw;
}

template <typename D>
D* RenderComponent::GetDrawable() const
{
	static_assert(std::is_base_of_v<IDrawable, D>, "D must inherit from IDrawable");

	for (const auto& drawable : Drawables)
	{
		if (D* result = dynamic_cast<D*>(drawable.get()))
			return result;
	}
	return nullptr;
}