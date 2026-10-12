#pragma once
#include <map>
#include <Scene.h>

class SceneMgr
{
public:
	std::map<std::string, Scene*> scenes;
	Scene* currentScene;
	~SceneMgr();

	Scene AddScene(Scene& scene);
	void RemoveScene(const std::string& sceneName);
	void ChangeScene(const std::string& sceneName);

	bool CheckNameCollision(const std::string& sceneName) const;

};