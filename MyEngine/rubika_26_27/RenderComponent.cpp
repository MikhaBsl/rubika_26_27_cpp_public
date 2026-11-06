#include "RenderComponent.h"
#include <IDrawable.h>
#include <Imgui/imgui.h>
#include <RectangleRenderer.h>
#include <SpriteRenderer.h>
#include <TransformComponent.h>
#include <Entity.h>
#include <Globals.h>

RenderComponent::RenderComponent(Entity& parent) : IComponent(parent)
{
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::Start()
{
	Transform = GetParentEntity().GetComponent<TransformComponent>();

	for (DrawableInfo& info : Drawables)
		info.Drawable->Start();
}

void RenderComponent::Update(float fDeltaTime)
{
	if (Transform == nullptr)
		Transform = GetParentEntity().GetComponent<TransformComponent>();

	for (DrawableInfo& info : Drawables)
	{
		if (!info.Drawable->IsVisible())
			continue;

		sf::Transform world = Transform != nullptr ? Transform->GetMatrix() : sf::Transform::Identity;
		if (info.HasRelativeTransform)
			world = world * info.RelativeTransform;

		info.Drawable->SetWorldTransform(world);
		info.Drawable->Update(fDeltaTime);
	}
}

void RenderComponent::Destroy()
{
	for (DrawableInfo& info : Drawables)
		info.Drawable->Destroy();
}

void RenderComponent::Draw(sf::RenderWindow& window) const
{
	for (const DrawableInfo& info : Drawables)
	{
		if (info.Drawable->IsVisible())
			info.Drawable->Draw(window);
	}
}

const char* RenderComponent::GetName() const
{
	return "Render";
}

void RenderComponent::SetDrawableRelativePosition(const IDrawable* drawable, const sf::Vector2f& position)
{
	for (DrawableInfo& info : Drawables)
	{
		if (info.Drawable.get() == drawable)
		{
			info.RelativePosition = position;
			info.ComputeTransform();
			return;
		}
	}
}

void RenderComponent::SetDrawableRelativeRotation(const IDrawable* drawable, float rotation)
{
	for (DrawableInfo& info : Drawables)
	{
		if (info.Drawable.get() == drawable)
		{
			info.RelativeRotation = rotation;
			info.ComputeTransform();
			return;
		}
	}
}

void RenderComponent::SetDrawableRelativeScale(const IDrawable* drawable, const sf::Vector2f& scale)
{
	for (DrawableInfo& info : Drawables)
	{
		if (info.Drawable.get() == drawable)
		{
			info.RelativeScale = scale;
			info.ComputeTransform();
			return;
		}
	}
}

sf::FloatRect RenderComponent::GetLocalBounds() const
{
	bool first = true;
	sf::Vector2f minPos;
	sf::Vector2f maxPos;

	for (const DrawableInfo& info : Drawables)
	{
		sf::Vector2f size = info.Drawable->GetSize();
		sf::Vector2f topLeft = info.RelativePosition;
		sf::Vector2f bottomRight = topLeft + size;

		if (first)
		{
			minPos = topLeft;
			maxPos = bottomRight;
			first = false;
			continue;
		}

		minPos.x = std::min(minPos.x, topLeft.x);
		minPos.y = std::min(minPos.y, topLeft.y);
		maxPos.x = std::max(maxPos.x, bottomRight.x);
		maxPos.y = std::max(maxPos.y, bottomRight.y);
	}

	return sf::FloatRect(minPos, maxPos - minPos);
}

RenderComponent::DrawableInfo::DrawableInfo()
{
	FriendlyName = "";
	Drawable = nullptr;
	RelativePosition = sf::Vector2f();
	RelativeRotation = 0.f;
	RelativeScale = sf::Vector2f(1.f, 1.f);
	RelativeTransform = sf::Transform::Identity;
	HasRelativeTransform = false;
}

void RenderComponent::DrawableInfo::ComputeTransform()
{
	RelativeTransform = sf::Transform::Identity;
	RelativeTransform.translate(RelativePosition);
	RelativeTransform.rotate(sf::degrees(RelativeRotation));
	RelativeTransform.scale(RelativeScale);

	HasRelativeTransform = RelativeTransform != sf::Transform::Identity;
}

#ifdef _USE_IMGUI
static bool DrawTextureCombo(const char* label, const std::string& current, bool animations, std::string& selected)
{
	bool changed = false;

	if (ImGui::BeginCombo(label, current.empty() ? "None" : current.c_str()))
	{
		for (const auto& [name, data] : gData.TextureMgr->GetTextures())
		{
			const bool hasData = animations ? !data.AnimationsData.empty() : !data.StaticTilesData.empty();
			if (!hasData)
				continue;

			if (ImGui::Selectable(name.c_str(), name == current))
			{
				selected = name;
				changed = true;
			}
		}
		ImGui::EndCombo();
	}

	return changed;
}
#endif

void RenderComponent::DrawDebug()
{
#ifdef _USE_IMGUI
	if (ImGui::Button("Add Rectangle"))
		AddDrawable<RectangleRenderer>("Rectangle " + std::to_string(Drawables.size()));

	ImGui::SameLine();
	if (ImGui::Button("Add Sprite"))
		AddDrawable<SpriteRenderer>("Sprite " + std::to_string(Drawables.size()));

	ImGui::Text("Drawables: %zu", Drawables.size());

	for (size_t i = 0; i < Drawables.size(); ++i)
	{
		DrawableInfo& info = Drawables[i];
		IDrawable* drawable = info.Drawable.get();
		ImGui::PushID(static_cast<int>(i));

		if (ImGui::TreeNode(info.FriendlyName.c_str()))
		{
			bool visible = drawable->IsVisible();
			if (ImGui::Checkbox("Visible", &visible))
				drawable->SetVisibility(visible);

			float pos[2] = { info.RelativePosition.x, info.RelativePosition.y };
			if (ImGui::DragFloat2("Offset", pos))
				SetDrawableRelativePosition(drawable, sf::Vector2f(pos[0], pos[1]));

			float rotation = info.RelativeRotation;
			if (ImGui::DragFloat("Relative rotation", &rotation))
				SetDrawableRelativeRotation(drawable, rotation);

			float scale[2] = { info.RelativeScale.x, info.RelativeScale.y };
			if (ImGui::DragFloat2("Relative scale", scale, 0.05f))
				SetDrawableRelativeScale(drawable, sf::Vector2f(scale[0], scale[1]));

			if (RectangleRenderer* rect = dynamic_cast<RectangleRenderer*>(drawable))
			{
				std::string selectedTexture;
				if (DrawTextureCombo("Texture", rect->GetCurrentTexture(), false, selectedTexture))
					rect->SetTexture(selectedTexture);

				const std::string currentTile = rect->GetCurrentTile();
				if (ImGui::BeginCombo("Tile", currentTile.empty() ? "None" : currentTile.c_str()))
				{
					if (!rect->GetCurrentTexture().empty())
					{
						const TextureData& textureData = gData.TextureMgr->GetTextureData(rect->GetCurrentTexture());
						for (const auto& [tileName, tile] : textureData.StaticTilesData)
						{
							if (ImGui::Selectable(tileName.c_str(), tileName == currentTile))
								rect->SetStaticTile(tileName);
						}
					}
					ImGui::EndCombo();
				}

				sf::Vector2f size = rect->GetSize();
				ImGui::Text("Size: %.0f x %.0f", size.x, size.y);

				sf::Color c = rect->GetFillColor();
				float color[4] = { c.r / 255.f, c.g / 255.f, c.b / 255.f, c.a / 255.f };
				if (ImGui::ColorEdit4("Color", color))
				{
					rect->SetFillColor(sf::Color(
						static_cast<std::uint8_t>(color[0] * 255.f),
						static_cast<std::uint8_t>(color[1] * 255.f),
						static_cast<std::uint8_t>(color[2] * 255.f),
						static_cast<std::uint8_t>(color[3] * 255.f)));
				}
			}
			else if (SpriteRenderer* sprite = dynamic_cast<SpriteRenderer*>(drawable))
			{
				std::string selectedTexture;
				if (DrawTextureCombo("Texture", sprite->GetCurrentTexture(), true, selectedTexture))
					sprite->SetTexture(selectedTexture);

				const std::string currentAnimation = sprite->GetCurrentAnimation();
				if (ImGui::BeginCombo("Animation", currentAnimation.empty() ? "None" : currentAnimation.c_str()))
				{
					if (!sprite->GetCurrentTexture().empty())
					{
						const TextureData& textureData = gData.TextureMgr->GetTextureData(sprite->GetCurrentTexture());
						for (const auto& [animationName, animation] : textureData.AnimationsData)
						{
							if (ImGui::Selectable(animationName.c_str(), animationName == currentAnimation))
								sprite->SetAnimation(animationName);
						}
					}
					ImGui::EndCombo();
				}

				bool play = sprite->IsAnimationEnabled();
				if (ImGui::Checkbox("Play", &play))
					sprite->EnableAnimation(play);

				ImGui::SameLine();
				if (ImGui::Button("Reset"))
					sprite->Reset();

				ImGui::Text("Frame: %d", sprite->GetCurrentFrame());

				sf::Vector2f size = sprite->GetSize();
				ImGui::Text("Size: %.0f x %.0f", size.x, size.y);
			}

			ImGui::TreePop();
		}

		ImGui::PopID();
	}
#endif
}