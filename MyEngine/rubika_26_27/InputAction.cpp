#include "InputAction.h"

InputAction::InputAction(const std::string& name)
{
	SetActionName(name);
}

void InputAction::SetActionName(const std::string& name)
{
	ActionName = name;
	ActionNameHash = std::hash<std::string>{}(name);
}

void InputAction::SetValue(float value)
{
	if (Value == value)
		return;

	Value = value;

	for (const sCallbackInfo& info : Callbacks)
	{
		info.Callback(Value);
	}
}

void InputAction::RegisterCallback(const std::string& debugName, std::function<void(float)> callback, void* owner)
{
	Callbacks.push_back({ debugName, callback, owner });
}

void InputAction::UnregisterCallbacks(void* owner)
{
	Callbacks.erase(std::remove_if(Callbacks.begin(), Callbacks.end(), [owner](const sCallbackInfo& info)
	{
		return info.Owner == owner;
	}), Callbacks.end());
}

std::string InputAction::GetName() const
{
	return ActionName;
}

std::size_t InputAction::GetNameHash() const
{
	return ActionNameHash;
}

float InputAction::GetValue() const
{
	return Value;
}
