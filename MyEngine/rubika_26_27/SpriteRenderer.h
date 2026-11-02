#pragma once
#include <IDrawable.h>
#include <TextureMgr.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include <string>

class SpriteRenderer : public IDrawable
{
public:
	std::unique_ptr<sf::Sprite> Sprite;

	SpriteRenderer();
	~SpriteRenderer();

	// Hérité via IDrawable
	void Start() override;
	void Update(float deltaTime) override;
	void Destroy() override;

	bool SetTexture(const std::string& textureName);
	bool SetAnimation(const std::string& animationName);
	bool SetTextureAndAnimation(const std::string& textureName, const std::string& animationName);
	void SetTextureRect(const sf::IntRect& rect);

	void EnableAnimation(bool enable);
	bool IsAnimationEnabled() const;
	void Reset();

	sf::Vector2f GetSize() const;
	const std::string& GetCurrentTexture() const;
	const std::string& GetCurrentAnimation() const;
	int GetCurrentFrame() const;

protected:
	void ApplyFrame();

	AnimationData CurrentAnimationData;

	std::string CurrentTexture;
	std::string CurrentAnimation;

	int CurrentAnimationNb = 0;
	float CurrentAnimationTime = 0.f;

	bool PlayAnimation = true;
};
