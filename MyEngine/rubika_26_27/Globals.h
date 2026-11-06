#pragma once
#include <SceneMgr.h>
#include <TextureMgr.h>
#include <InputMgr.h>
#include <CollisionMgr.h>

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
	CollisionMgr* CollisionMgr;

	bool DrawDebugColliders = true;
};

extern Globals gData;