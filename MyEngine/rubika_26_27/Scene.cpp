#include <Scene.h>
#include <algorithm>


Scene::Scene(const std::string& name)
{
	sceneName = name;
}

Scene::~Scene()
{
}

void Scene::Start()
{
	for (Entity* entity : entities)
		entity->Start();
}

void Scene::Update(float fDeltaTime)
{
	for (Entity* entity : entities)
		entity->Update(fDeltaTime);
}

void Scene::Destroy()
{
	for (Entity* entity : entities)
	{
		delete entity;
	}

	entities.clear();
}

Entity& Scene::AddEntity(Entity* entity)
{
	entities.push_back(entity);
	return *entity;
}

void Scene::RemoveEntity(Entity& entity)
{
	entities.erase(std::remove(entities.begin(), entities.end(), &entity), entities.end());
	delete& entity;
}

void Scene::Draw(sf::RenderWindow& window) const
{
	for (Entity* entity : entities)
		entity->Draw(window);
}


