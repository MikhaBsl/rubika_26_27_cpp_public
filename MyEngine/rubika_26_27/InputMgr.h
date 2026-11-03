#pragma once

#include <DebugMgr.h>
#include <InputAction.h>
#include <map>

class InputMgr : public IDebugable
{
public:
	InputMgr();
	~InputMgr();

	void Init();
	void Update();
	void Shut();

	void DrawDebug() override;

	InputAction* CreateAction(const std::string& name);
	void RemoveAction(const std::string& name);
	void RemoveAction(std::size_t hash);
	InputAction* GetAction(std::size_t hash);

	std::map<std::size_t, InputAction> InputActions;
};