#include "SceneMgr.h"
#include "stdexcept"
#include <Imgui/imgui.h>

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
	auto it = scenes.find(sceneName);
	if (it != scenes.end())
	{
		delete it->second;
		scenes.erase(it);
	}
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

void SceneMgr::DrawSceneView()
{
#ifdef _USE_IMGUI
	ImGui::Begin("Scene Manager");

	if (currentScene != nullptr)
	{
		ImGui::Text("Active Scene: %s", currentScene->sceneName.c_str());
	}
	else
	{
		ImGui::Text("Active Scene: None");
	}

	ImGui::Separator();

	if (ImGui::Button("Add Scene"))
	{

	}
	ImGui::SameLine();
	if (ImGui::Button("Remove Scene"))
	{

	}
	ImGui::SameLine();
	if (ImGui::Button("Change Scene"))
	{

	}
	#endif
}
