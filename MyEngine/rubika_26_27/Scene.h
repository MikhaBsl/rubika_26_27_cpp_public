#pragma once

#include <string>
#include <vector>
#include <Entity.h>

namespace sf
{
	class RenderWindow;
}

class Scene
{
public:
	std::string sceneName;
	std::vector<Entity*> entities;

	Scene(const std::string& name);
	~Scene();

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	Entity& AddEntity(Entity* entity);;
	void RemoveEntity(Entity& entity);

	/*
	 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	 * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;
};