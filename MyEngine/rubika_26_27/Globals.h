#pragma once
#include <SceneMgr.h>
#include <TextureMgr.h>
#include <InputMgr.h>

class DebugMgr;
class Logger;

class Globals
{
public:
	Globals();
	~Globals();

	void Init();
	void Update(float deltaTime);
	void Shut();
	void Destroy();

	void IncrementFrameCount();

	unsigned long long FrameCount;

	DebugMgr* DebugMgr;
	Logger* Logger;
	SceneMgr* SceneMgr;
	TextureMgr* TextureMgr;
	InputMgr* InputMgr;
};

extern Globals gData;