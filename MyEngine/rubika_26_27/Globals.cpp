#include "Globals.h"

#include <DebugMgr.h>
#include <Logger.h>

Globals gData;

Globals::Globals() : FrameCount(0)
{
	InputMgr = new ::InputMgr();
	DebugMgr = new ::DebugMgr();
	Logger = new ::Logger();
	SceneMgr = new::SceneMgr();
	TextureMgr = new::TextureMgr();
}

Globals::~Globals()
{
	Destroy();
}

void Globals::Init()
{
	InputMgr->Init();
	Logger->Init();
	SceneMgr->Init();
	TextureMgr->Init();
}

void Globals::Shut()
{
	InputMgr->Shut();
	Logger->Shut();
	SceneMgr->Shut();
	TextureMgr->Shut();
}

void Globals::Destroy()
{
	delete DebugMgr;
	DebugMgr = nullptr;

	delete Logger;
	Logger = nullptr;

	delete SceneMgr;
	SceneMgr = nullptr;

	delete TextureMgr;
	TextureMgr = nullptr;

	delete InputMgr;
	InputMgr = nullptr;
}

void Globals::IncrementFrameCount()
{
	++FrameCount;
}
