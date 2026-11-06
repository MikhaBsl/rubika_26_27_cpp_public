#pragma once
#include <Entity.h>

class CollisionMgr
{
public:
	CollisionMgr();
	~CollisionMgr();

	void Update(float fDeltaTime);

	void CheckCollisions(Entity* entity);
};