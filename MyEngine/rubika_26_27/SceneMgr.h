#pragma once
#include <map>
#include <Scene.h>
#include <DebugMgr.h>

class SceneMgr : public IDebugable
{
public:
	std::map<std::string, Scene*> scenes;
	Scene* currentScene;
	std::string selectedSceneName;
	char newSceneName[64] = "";
	char newEntityName[64] = "";

	SceneMgr();
	~SceneMgr();

	Scene* AddScene(Scene* scene);
	void RemoveScene(const std::string& sceneName);
	void ChangeScene(const std::string& sceneName);

	bool CheckNameCollision(const std::string& sceneName) const;

	void Init();
	void Shut();

	virtual void DrawDebug() override;

	void Update(float deltaTime);
	void Draw(sf::RenderWindow window) const;
};