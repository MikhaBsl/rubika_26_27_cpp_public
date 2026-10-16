#include "RenderComponent.h"
#include <IDrawable.h>
#include <Imgui/imgui.h>
#include <RectangleRenderer.h>
#include <SpriteRenderer.h>
#include <Globals.h>

RenderComponent::RenderComponent(Entity& parent) : IComponent(parent)
{
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::Start()
{
	for(auto& drawable : Drawables)
		drawable->Start();
}

void RenderComponent::Update(float fDeltaTime)
{
	for (auto& drawable : Drawables)
		drawable->Update(fDeltaTime);
}

void RenderComponent::Destroy()
{
	for (auto& drawable : Drawables)
		drawable->Destroy();
}

void RenderComponent::Draw(sf::RenderWindow& window) const
{
	for (auto& drawable : Drawables)
		drawable->Draw(window);
}

const char* RenderComponent::GetName() const
{
	return "Render";
}

void RenderComponent::SetWorldTransform(const sf::Transform& transform)
{
	for (auto& drawable : Drawables)
		drawable->SetWorldTransform(transform);
}

void RenderComponent::DrawDebug()
{
#ifdef _USE_IMGUI
	if (ImGui::Button("Add Rectangle"))
	{
		RectangleRenderer* rect = AddDrawable<RectangleRenderer>();
		rect->SetSize(sf::Vector2f(100.f, 60.f));
		rect->SetFillColor(sf::Color::Red);
	}

	if (ImGui::Button("Add Sprite"))
		ImGui::OpenPopup("AddSpritePopup");

	if (ImGui::BeginPopup("AddSpritePopup"))
	{
		for (const auto& [name, data] : gData.TextureMgr->textures)
		{
			if (ImGui::MenuItem(name.c_str()))
			{
				SpriteRenderer* sprite = AddDrawable<SpriteRenderer>();
				sprite->SetTexture(data.Texture);
			}
		}
		ImGui::EndPopup();
	}

	ImGui::Text("Drawables: %zu", Drawables.size());

	for (size_t i = 0; i < Drawables.size(); ++i)
	{
		IDrawable* drawable = Drawables[i].get();
		ImGui::PushID(static_cast<int>(i));

		if (RectangleRenderer* rect = dynamic_cast<RectangleRenderer*>(drawable))
		{
			if (ImGui::TreeNode("Rectangle"))
			{
				sf::Vector2f size = rect->GetSize();
				float sizeArr[2] = { size.x, size.y };
				if (ImGui::DragFloat2("Size", sizeArr))
					rect->SetSize(sf::Vector2f(sizeArr[0], sizeArr[1]));

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

				bool visible = rect->IsVisible();
				if (ImGui::Checkbox("Visible", &visible))
					rect->SetVisibility(visible);

				ImGui::TreePop();
			}
		}

		else if (SpriteRenderer* sprite = dynamic_cast<SpriteRenderer*>(drawable))
		{
			if (ImGui::TreeNode("Sprite"))
			{
				sf::Vector2f size = sprite->GetSize();
				ImGui::Text("Size: %.0f x %.0f", size.x, size.y);

				bool visible = sprite->IsVisible();
				if (ImGui::Checkbox("Visible", &visible))
					sprite->SetVisibility(visible);

				ImGui::TreePop();
			}
		}

		ImGui::PopID();
	}
#endif
}