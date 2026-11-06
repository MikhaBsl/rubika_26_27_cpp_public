#include "CollisionMgr.h"
#include <Scene.h>
#include <Globals.h>
#include <DynamicColliderComponent.h>

CollisionMgr::CollisionMgr()
{
}

CollisionMgr::~CollisionMgr()
{
}

void CollisionMgr::Update(float fDeltaTime)
{
	Scene* scene = gData.SceneMgr->GetCurrentScene();
	if (scene == nullptr)
		return;

	for (Entity* entity : scene->entities)
		CheckCollisions(entity);
}

void CollisionMgr::CheckCollisions(Entity* entity)
{
	DynamicColliderComponent* collider = entity->GetComponent<DynamicColliderComponent>();
	if (collider == nullptr || !collider->AreCollisionsEnabled())
		return;

	for (Entity* other : gData.SceneMgr->GetCurrentScene()->entities)
	{
		if (other == entity)
			continue;

		DynamicColliderComponent* otherCollider = other->GetComponent<DynamicColliderComponent>();
		if (otherCollider == nullptr || !otherCollider->AreCollisionsEnabled())
			continue;

		std::optional<sf::FloatRect> intersection = collider->GetHitbox().findIntersection(otherCollider->GetHitbox());
		if (!intersection)
			continue;

		if (collider->OnCollision(other) != OnCollisionChoice::Block)
			continue;

		TransformComponent* transform = entity->GetComponent<TransformComponent>();
		if (transform == nullptr)
			continue;

		OnCollisionChoice choice = collider->OnCollision(other);
		//Logger::Info(entity->name + " collision with " + other->name);

		const sf::FloatRect& a = collider->GetHitbox();
		const sf::FloatRect& b = otherCollider->GetHitbox();

		sf::Vector2f centerA = a.position + a.size / 2.f;
		sf::Vector2f centerB = b.position + b.size / 2.f;
		sf::Vector2f correction(0.f, 0.f);

		if (intersection->size.x < intersection->size.y)
			correction.x = centerA.x < centerB.x ? -intersection->size.x : intersection->size.x;
		else
			correction.y = centerA.y < centerB.y ? -intersection->size.y : intersection->size.y;

		transform->SetWorldPosition(transform->GetWorldPosition() + correction);
		collider->UpdateHitboxPosition();
	}
}
