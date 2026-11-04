#include "InputMgr.h"
#include <Globals.h>
#include <algorithm>
#include <unordered_map>

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#endif

InputMgr::InputMgr()
{
}

InputMgr::~InputMgr()
{
}

void InputMgr::Init()
{
	gData.DebugMgr->RegisterDebugableWindow("Input Manager", this);
}

void InputMgr::Update(float deltaTime)
{
	std::unordered_map<std::size_t, float> values;

	for (const auto& [key, hashes] : KeyToAction)
	{
		const bool pressed = sf::Keyboard::isKeyPressed(key);
		for (std::size_t hash : hashes)
			values[hash] = std::max(values[hash], pressed ? 1.f : 0.f);
	}

	for (const auto& [hash, value] : values)
	{
		if (InputAction* action = GetAction(hash))
			action->SetValue(value);
	}
}

void InputMgr::Shut()
{
	gData.DebugMgr->UnregisterDebugableWindow("Input Manager");
}

void InputMgr::DrawDebug()
{
	ImGui::Text("Actions: %zu", InputActions.size());
	ImGui::Separator();

	for (auto& [hash, action] : InputActions)
	{
		if (ImGui::TreeNode(action.GetName().c_str()))
		{
			ImGui::Text("Value: %.2f", action.GetValue());
			ImGui::Text("Callbacks: %zu", action.GetCallbacks().size());

			for (const auto& callback : action.GetCallbacks())
				ImGui::BulletText("%s", callback.DebugName.c_str());

			ImGui::TreePop();
		}
	}
}

InputAction* InputMgr::CreateAction(const std::string& name)
{
	auto [it, inserted] = InputActions.emplace(std::hash<std::string>{}(name), InputAction(name));
	return inserted ? &it->second : nullptr;
}

void InputMgr::RemoveAction(const std::string& name)
{
	RemoveAction(std::hash<std::string>{}(name));
}

void InputMgr::RemoveAction(std::size_t hash)
{
	InputActions.erase(hash);
}

InputAction* InputMgr::GetAction(std::size_t hash)
{
	auto it = InputActions.find(hash);
	return it != InputActions.end() ? &it->second : nullptr;
}

InputAction* InputMgr::GetAction(const std::string& name)
{
	return GetAction(std::hash<std::string>{}(name));
}

bool InputMgr::RegisterToAction(std::size_t hash, const std::string& debugName, std::function<void(float)> callback, void* owner)
{
	InputAction* action = GetAction(hash);
	if (action == nullptr)
		return false;

	action->RegisterCallback(debugName, callback, owner);
	return true;
}

bool InputMgr::RegisterToAction(const std::string& name, const std::string& debugName, std::function<void(float)> callback, void* owner)
{
	InputAction* action = GetAction(name);
	if (action == nullptr)
		return false;

	action->RegisterCallback(debugName, callback, owner);
	return true;
}

bool InputMgr::UnregisterFromAction(std::size_t hash, void* owner)
{
	InputAction* action = GetAction(hash);
	if (action == nullptr)
		return false;

	action->UnregisterCallbacks(owner);
	return true;
}

bool InputMgr::UnregisterFromAction(const std::string& name, void* owner)
{
	InputAction* action = GetAction(name);
	if (action == nullptr)
		return false;

	action->UnregisterCallbacks(owner);
	return true;
}

void InputMgr::SetActionKey(std::size_t hash, sf::Keyboard::Key key)
{
	InputAction* action = GetAction(hash);
	if (action == nullptr)
		return;

	std::vector<std::size_t>& hashes = KeyToAction[static_cast<sf::Keyboard::Key>(key)];
	if (std::find(hashes.begin(), hashes.end(), hash) == hashes.end())
		hashes.push_back(hash);
}

void InputMgr::SetActionKey(const std::string& name, sf::Keyboard::Key key)
{
	SetActionKey(std::hash<std::string>{}(name), key);
}

void InputMgr::RemoveKeyBinding(sf::Keyboard::Key key, InputAction& action)
{
	auto it = KeyToAction.find(key);
	if (it != KeyToAction.end())
	{
		auto& hashes = it->second;
		hashes.erase(std::remove(hashes.begin(), hashes.end(), action.GetNameHash()), hashes.end());
		if (hashes.empty())
			KeyToAction.erase(it);
	}
}

void InputMgr::RemoveKeyBinding(sf::Keyboard::Key key, std::size_t hash)
{
	InputAction* action = GetAction(hash);
	if (action == nullptr)
		return;

	RemoveKeyBinding(key, *action);
}

void InputMgr::RemoveKeyBinding(sf::Keyboard::Key key, const std::string& actionName)
{
	RemoveKeyBinding(key, std::hash<std::string>{}(actionName));
}

const std::map<sf::Keyboard::Key, std::vector<std::size_t>>& InputMgr::GetBindings() const
{
	return KeyToAction;
}
