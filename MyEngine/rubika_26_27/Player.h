#pragma once
#include "Entity.h"
#include <PlayerController.h>

class Player : public Entity
{
public :
	Player(std::string friendlyName = "Player");
	~Player() override;

	void Start() override ;
	void Update(float fDeltaTime) override;
	void Destroy() override;

	void DrawPlayer() const;
private:
	PlayerController* controller = nullptr;
};