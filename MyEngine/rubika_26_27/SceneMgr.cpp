#include "SceneMgr.h"
#include "stdexcept"
#include <Imgui/imgui.h>
#include <Globals.h>

SceneMgr::SceneMgr()
{
	currentScene = nullptr;
}

SceneMgr::~SceneMgr()
{
	for (auto& pair : scenes)
	{
		delete pair.second;
	}
}

Scene* SceneMgr::AddScene(Scene* scene)
{
	if (CheckNameCollision(scene->sceneName) || scene == nullptr)
		return nullptr;

	auto it = scenes.emplace(scene->sceneName, scene);
	return it.first->second;
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

void SceneMgr::Init()
{
	gData.DebugMgr->RegisterDebugableWindow("Scene Manager", this);
}

void SceneMgr::Shut()
{
	gData.DebugMgr->UnregisterDebugableWindow("Scene Manager");
}

void SceneMgr::DrawDebug()
{
#ifdef _USE_IMGUI

	if (currentScene != nullptr)
	{
		ImGui::Text("Active Scene: %s", currentScene->sceneName.c_str());
	}
	else
	{
		ImGui::Text("Active Scene: None");
	}
	
	ImGui::Separator();

	ImGui::InputTextWithHint("##NewSceneName", "New scene name", newSceneName, sizeof(newSceneName));

	if (ImGui::Button("Add Scene"))
	{
		if (newSceneName[0] != '\0' && !CheckNameCollision(newSceneName))
		{
			AddScene(new Scene(newSceneName));
			newSceneName[0] = '\0';
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove Scene"))
	{
		if (!selectedSceneName.empty())
			RemoveScene(selectedSceneName);
	}
	ImGui::SameLine();
	if (ImGui::Button("Change Scene"))
	{
		if(!selectedSceneName.empty()) 
			ChangeScene(selectedSceneName);
	}

	ImGui::Separator();

	auto it = scenes.find(selectedSceneName);
	if (it != scenes.end())
	{
		Scene* scene = it->second;

		ImGui::Text("Scene: %s", scene->sceneName.c_str());
		ImGui::Text("Entities: %zu", scene->entities.size());

		ImGui::InputTextWithHint("##NewEntityName", "Entity name", newEntityName, sizeof(newEntityName));
		ImGui::SameLine();

		if (ImGui::Button("Add Entity"))
		{
			scene->AddEntity(new Entity(newEntityName[0] != '\0' ? newEntityName : "Entity"));
			newEntityName[0] = '\0';
		}

		Entity* entityToRemove = nullptr;

		for (Entity* entity : scene->entities)
		{
			if (entity == nullptr)
				continue;

			if (ImGui::TreeNode(entity, "%s", entity->name.c_str()))
			{
				//ImGui::Text("Address: %p", (void*)entity);
				if (ImGui::Button("Remove Entity"))
					entityToRemove = entity;
				ImGui::TreePop();
			}
		}

		if (entityToRemove != nullptr)
			scene->RemoveEntity(*entityToRemove);
	}
	else
	{
		ImGui::TextDisabled("Select a scene to see its details");
	}

	ImGui::Separator();

	for (auto& [name, scene] : scenes)
	{
		bool isActive = (scene == currentScene);
		std::string label = isActive ? name + " (active)" : name;

		if (ImGui::Selectable(label.c_str(), selectedSceneName == name))
		{
			selectedSceneName = name;
		}
	}
	#endif
}
