#pragma once

#include <Logger.h>
#include <type_traits>
#include <typeinfo>

template <typename C>
C* Entity::GetComponent() const
{
	static_assert(std::is_base_of_v<IComponent, C>, "C must inherit from IComponent");

	for (const auto& component : components)
	{
		if (C* result = dynamic_cast<C*>(component.get()))
			return result;
	}
	return nullptr;
}

template <typename C>
C* Entity::AddComponent()
{
	static_assert(std::is_base_of_v<IComponent, C>, "C must inherit from IComponent");
	
	if (GetComponent<C>() != nullptr)
	{
		Logger::Warning(std::string("Component already present : " + typeid(C).name()));
		return nullptr;
	}

	auto component = std::make_unique<C>(*this);
	C* raw = component.get();
	components.push_back(std::move(component));
	return raw;
}