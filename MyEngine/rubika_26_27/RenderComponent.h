#pragma once
#include <IComponent.h>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <string>
#include<IDrawable.h>

class TransformComponent;

class RenderComponent : public IComponent
{
public:
	RenderComponent(Entity& parent);
	~RenderComponent() override;

	const char* GetName() const override;
	void DrawDebug() override;

	void Start() override;
	void Update(float fDeltaTime) override;
	void Destroy() override;

	void Draw(sf::RenderWindow& window) const;

	template <typename D>
	D* AddDrawable(const std::string& friendlyName);

	template <typename D>
	D* AddDrawable(const std::string& friendlyName, sf::Vector2f relativePos, float relativeAngle, sf::Vector2f relativeScale);

	template<typename D>
	D* GetDrawable() const;

	void SetDrawableRelativePosition(const IDrawable* drawable, const sf::Vector2f& position);
	void SetDrawableRelativeRotation(const IDrawable* drawable, float rotation);
	void SetDrawableRelativeScale(const IDrawable* drawable, const sf::Vector2f& scale);

	sf::FloatRect GetLocalBounds() const;

protected:

	struct DrawableInfo
	{
		DrawableInfo();

		std::string FriendlyName;
		std::unique_ptr<IDrawable> Drawable;
		sf::Vector2f RelativePosition;
		float RelativeRotation;
		sf::Vector2f RelativeScale;
		sf::Transform RelativeTransform;
		bool HasRelativeTransform;

		void ComputeTransform();
	};

	std::vector<DrawableInfo> Drawables;
	TransformComponent* Transform = nullptr;
};

#include "RenderComponent.hxx"
