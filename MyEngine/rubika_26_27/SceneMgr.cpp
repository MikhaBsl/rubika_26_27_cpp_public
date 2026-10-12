#include "SceneMgr.h"
#include "stdexcept"

SceneMgr::~SceneMgr()
{
	for (auto& pair : scenes)
	{
		delete pair.second;
	}
}

Scene SceneMgr::AddScene(Scene& scene)
{
	if (CheckNameCollision(scene.sceneName)) 
		throw std::runtime_error("Erreur : Une scene avec ce nom existe deja.");

	Scene* newScene = new Scene(scene);

	auto it = scenes.emplace(scene.sceneName, newScene);
	return *(it.first->second);
}

void SceneMgr::RemoveScene(const std::string& sceneName)
{
	scenes.erase(sceneName);
}

void SceneMgr::ChangeScene(const std::string& sceneName)
{
	const auto& it = scenes.find(sceneName);
	if (it == scenes.end())
	{
		return;
	}
	currentScene = it->second;
}

bool SceneMgr::CheckNameCollision(const std::string& sceneName) const
{
	return scenes.contains(sceneName);
}
