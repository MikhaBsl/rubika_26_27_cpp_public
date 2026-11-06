#pragma once
#include "Entity.h"

class Obstacle : public Entity
{
public:
	Obstacle( std::string fileName, std::string tileName, std::string friendlyName = "Obstacle");
	~Obstacle() override;

	void Start() override;
	void Update(float fDeltaTime) override;
	void Destroy() override;

	void DrawObstacle(std::string fileName, std::string tileName) const;
};