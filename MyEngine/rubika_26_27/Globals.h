#pragma once
#include <SceneMgr.h>
#include <TextureMgr.h>

class DebugMgr;
class Logger;

class Globals
{
public:
	Globals();
	~Globals();

	void Init();
	void Shut();
	void Destroy();

	void IncrementFrameCount();

	unsigned long long FrameCount;

	DebugMgr* DebugMgr;
	Logger* Logger;
	SceneMgr* SceneMgr;
	TextureMgr* TextureMgr;
};

extern Globals gData;