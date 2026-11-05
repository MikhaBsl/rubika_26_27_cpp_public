#include "DynamicColliderComponent.h"
#include "Entity.h"
#include "TransformComponent.h"
#include <Imgui/imgui.h>
#include <Globals.h>

DynamicColliderComponent::DynamicColliderComponent(Entity& entity) : IComponent(entity)
{

}

DynamicColliderComponent::~DynamicColliderComponent()
{
}

void DynamicColliderComponent::Start()
{
}

void DynamicColliderComponent::Update(float fDeltaTime)
{
	UpdateHitboxPosition();
}

void DynamicColliderComponent::Destroy()
{
}

void DynamicColliderComponent::SetHitboxSize(sf::Vector2f size)
{
	Hitbox.size = size;
}

void DynamicColliderComponent::SetHitboxOffset(sf::Vector2f offset)
{
	HitboxOffset = offset;
}

void DynamicColliderComponent::EnableCollision(bool bEnable)
{
	bCollisionEnabled = bEnable;
}

bool DynamicColliderComponent::AreCollisionsEnabled() const
{
	return bCollisionEnabled;
}

const sf::FloatRect& DynamicColliderComponent::GetHitbox() const
{
	return Hitbox;
}

void DynamicColliderComponent::SetOnCollisionCallback(std::function<OnCollisionChoice(const Entity*)> callback)
{
	OnCollisionCallback = callback;
}

const char* DynamicColliderComponent::GetName() const
{
	return "Collider";
}

void DynamicColliderComponent::DrawDebug()
{
#ifdef _USE_IMGUI


	float size[2] = { Hitbox.size.x, Hitbox.size.y };
	if (ImGui::DragFloat2("Size", size))
		SetHitboxSize(sf::Vector2f(size[0], size[1]));

	float offset[2] = { HitboxOffset.x, HitboxOffset.y };
	if (ImGui::DragFloat2("Offset", offset))
		SetHitboxOffset(sf::Vector2f(offset[0], offset[1]));

	bool enabled = bCollisionEnabled;
	if (ImGui::Checkbox("Collisions enabled", &enabled))
		EnableCollision(enabled);

	ImGui::Text("Position: %.1f, %.1f", Hitbox.position.x, Hitbox.position.y);

	if(gData.DrawDebugColliders)
		Draw();

#endif // _USE_IMGUI
}

void DynamicColliderComponent::Draw() const
{
#ifdef _USE_IMGUI
	ImColor color = ImColor(0, 255, 0, 255);
	ImDrawList* draw_list = ImGui::GetForegroundDrawList();

	ImVec2 p_min = ImVec2(Hitbox.position.x, Hitbox.position.y);
	ImVec2 p_max = ImVec2(Hitbox.position.x + Hitbox.size.x, Hitbox.position.y + Hitbox.size.y);

	draw_list->AddRect(p_min, p_max, color, 0.0f, 0, 2.0f);
#endif
}

void DynamicColliderComponent::UpdateHitboxPosition()
{
	if (Transform == nullptr)
	{
		Transform = GetParentEntity().GetComponent<TransformComponent>();
	}

	if (Transform != nullptr)
	{
		Hitbox.position = Transform->GetWorldPosition() + HitboxOffset;
	}
	else
		Hitbox.position = HitboxOffset;
}
