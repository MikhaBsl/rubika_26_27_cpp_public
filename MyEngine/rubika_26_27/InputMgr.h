#pragma once

#include <DebugMgr.h>
#include <InputAction.h>
#include <map>
#include <SFML/Window/Keyboard.hpp>

class InputMgr : public IDebugable
{
public:
	InputMgr();
	~InputMgr();

	void Init();
	void Update(float deltaTime);
	void Shut();

	void DrawDebug() override;

	InputAction* CreateAction(const std::string& name);
	void RemoveAction(const std::string& name);
	void RemoveAction(std::size_t hash);
	InputAction* GetAction(std::size_t hash);

	InputAction* GetAction(const std::string& name);

	bool RegisterToAction(std::size_t hash, const std::string& debugName, std::function<void(float)> callback, void* owner);
	bool RegisterToAction(const std::string& name, const std::string& debugName, std::function<void(float)> callback, void* owner);

	bool UnregisterFromAction(std::size_t hash, void* owner);
	bool UnregisterFromAction(const std::string& name, void* owner);

	void SetActionKey(std::size_t hash, sf::Keyboard::Key key);
	void SetActionKey(const std::string& name, sf::Keyboard::Key key);


	void RemoveKeyBinding(sf::Keyboard::Key key, InputAction& action);
	void RemoveKeyBinding(sf::Keyboard::Key key, std::size_t hash);
	void RemoveKeyBinding(sf::Keyboard::Key key, const std::string& actionName);

	const std::map<sf::Keyboard::Key, std::vector<std::size_t>>& GetBindings() const;

	std::map<std::size_t, InputAction> InputActions;
	std::map<sf::Keyboard::Key, std::vector<std::size_t>> KeyToAction;
};