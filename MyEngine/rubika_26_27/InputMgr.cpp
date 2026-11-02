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
