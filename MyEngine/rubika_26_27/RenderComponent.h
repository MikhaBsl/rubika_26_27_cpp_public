#pragma once
#include <IComponent.h>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include<IDrawable.h>


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
	D* AddDrawable();

	template<typename D>
	D* GetDrawable() const;

	void SetWorldTransform(const sf::Transform& transform);

private:
	std::vector<std::unique_ptr<IDrawable>> Drawables;

};

#include "RenderComponent.hxx"