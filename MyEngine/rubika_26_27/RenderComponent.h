#pragma once
#include <IComponent.h>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

class IDrawable;

class RenderComponent : public IComponent
{
public:
	RenderComponent(Entity& parent);
	~RenderComponent();

	void Start() override;
	void Update(float fDeltaTime) override;
	void Destroy() override;

	void SetDrawable(std::unique_ptr<IDrawable> drawable);
	void Draw(sf::RenderWindow& window) const;

	template <typename D>
	D* AddDrawable();

	template<typename D>
	D* GetDrawable() const;


private:
	std::unique_ptr<IDrawable> Drawable;
};

#include "RenderComponent.hxx"