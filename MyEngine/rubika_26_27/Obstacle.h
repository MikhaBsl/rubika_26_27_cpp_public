#pragma once
#include "Entity.h"

class Obstacle : public Entity
{
	Obstacle(std::string friendlyName = "Obstacle");
	~Obstacle() override;

	void Start() override;
	void Update(float fDeltaTime) override;
	void Destroy() override;
};