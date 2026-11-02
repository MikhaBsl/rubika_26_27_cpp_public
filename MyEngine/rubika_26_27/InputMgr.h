#pragma once

#include <DebugMgr.h>

class InputMgr : public IDebugable
{
public:
	InputMgr();
	~InputMgr();

	void Init();
	void Update();
	void Shut();

	void DrawDebug() override;
};