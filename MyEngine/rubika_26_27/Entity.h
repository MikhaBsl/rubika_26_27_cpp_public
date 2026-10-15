#pragma once

#include <string>
#include <IComponent.h>
#include <vector>
#include <memory>

namespace sf
{
	class RenderWindow;
}

class Entity
{
public:
	std::string name;

	Entity(std::string friendlyName = "");
	virtual ~Entity();

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	/*
	 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	 * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;

	template <typename C>
	C* AddComponent();

	template <typename C>
	C* GetComponent() const;

private:

	std::vector<std::unique_ptr<IComponent>> components;
};

#include "Entity.hxx"
