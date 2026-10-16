#include "TransformComponent.h"
#include <Imgui/imgui.h>

TransformComponent::TransformComponent(Entity& ParentComponent) : IComponent(ParentComponent)
{
	WorldPosition = sf::Vector2f();
	WorldPositionQuery = sf::Vector2f();
	Rotation = 0.f;
	Scale = sf::Vector2f(1.f, 1.f);

	UpdateMatrix();
}


TransformComponent::~TransformComponent()
{}

const sf::Transform& TransformComponent::GetMatrix() const
{
	return Matrix;
}

void TransformComponent::SetWorldPosition(sf::Vector2f newPosition)
{
	WorldPosition = newPosition;
	WorldPositionQuery = newPosition;
	UpdateMatrix();
}

sf::Vector2f TransformComponent::GetWorldPosition() const
{
	return WorldPosition;
}

void TransformComponent::SetWorldPositionQuery(sf::Vector2f newPosition)
{
	WorldPositionQuery = newPosition;
}

sf::Vector2f TransformComponent::GetWorldPositionQuery() const
{
	return WorldPositionQuery;
}

void TransformComponent::SetRotation(float newRotation)
{
	Rotation = newRotation;
	UpdateMatrix();
}

float TransformComponent::GetRotation() const
{
	return Rotation;
}

void TransformComponent::SetScale(sf::Vector2f newScale)
{
	Scale = newScale;
	UpdateMatrix();
}

sf::Vector2f TransformComponent::GetScale() const
{
	return Scale;
}

void TransformComponent::Start()
{}

void TransformComponent::Update(float fDeltaTime)
{}

void TransformComponent::Destroy()
{}

void TransformComponent::UpdateMatrix()
{
	Matrix = sf::Transform::Identity;
	Matrix.translate(WorldPosition);
	Matrix.rotate(sf::degrees(Rotation));
	Matrix.scale(Scale);
}

const char* TransformComponent::GetName() const
{
	return "Transform";
}

void TransformComponent::DrawDebug()
{
#ifdef _USE_IMGUI
	float pos[2] = { WorldPosition.x, WorldPosition.y };
	if (ImGui::DragFloat2("Position", pos))
		SetWorldPosition(sf::Vector2f(pos[0], pos[1]));

	float rotation = Rotation;
	if (ImGui::DragFloat("Rotation", &rotation))
		SetRotation(rotation);

	float scale[2] = { Scale.x, Scale.y };
	if (ImGui::DragFloat2("Scale", scale, 0.05f))
		SetScale(sf::Vector2f(scale[0], scale[1]));
#endif
}
