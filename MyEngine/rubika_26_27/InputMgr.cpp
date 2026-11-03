#include "InputMgr.h"
#include <Globals.h>

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

void InputMgr::Update()
{
}

void InputMgr::Shut()
{
	gData.DebugMgr->UnregisterDebugableWindow("Input Manager");
}

void InputMgr::DrawDebug()
{
#ifdef _USE_IMGUI
	ImGui::Text("Input Manager");
#endif
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
