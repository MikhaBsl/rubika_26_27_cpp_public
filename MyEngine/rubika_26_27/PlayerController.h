#pragma once
#include <TransformComponent.h>
class PlayerController
{
public:
	PlayerController(TransformComponent* transform);
	~PlayerController();

	void Init();
	void Update(float deltaTime);

private:
	void OnUp(float value);
	void OnDown(float value);
	void OnLeft(float value);
	void OnRight(float value);

	void OnDebug(float value);

	TransformComponent* Transform;
	sf::Vector2f MoveDir;
	float Speed = 200.f;
};