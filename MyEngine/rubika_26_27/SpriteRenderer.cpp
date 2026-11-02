#include "SpriteRenderer.h"
#include <Logger.h>
#include <Globals.h>

SpriteRenderer::SpriteRenderer()
{
	Drawable = nullptr;
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Start()
{
}

void SpriteRenderer::Update(float deltaTime)
{
	if (!Sprite)
		return;

	if (PlayAnimation && CurrentAnimationData.AnimationSpriteCount > 0)
	{
		CurrentAnimationTime += deltaTime;
		if (CurrentAnimationTime >= CurrentAnimationData.TimeBetweenAnimationInS)
		{
			++CurrentAnimationNb;
			if (CurrentAnimationNb >= CurrentAnimationData.AnimationSpriteCount)
				CurrentAnimationNb = 0;

			CurrentAnimationTime = 0.f;
		}
	}

	ApplyFrame();
}

void SpriteRenderer::Destroy()
{
}

bool SpriteRenderer::SetTexture(const std::string& textureName)
{
	if (!gData.TextureMgr->HasTexture(textureName))
	{
		Logger::Error("SpriteRenderer: unknown texture " + textureName);
		return false;
	}

	const TextureData& textureData = gData.TextureMgr->GetTextureData(textureName);

	if (!Sprite)
		Sprite = std::make_unique<sf::Sprite>(textureData.Texture);
	else
		Sprite->setTexture(textureData.Texture, true);

	Drawable = Sprite.get();

	if (CurrentTexture != textureName)
	{
		CurrentAnimation.clear();
		CurrentAnimationData = AnimationData();
		Reset();
	}

	CurrentTexture = textureName;
	return true;
}

bool SpriteRenderer::SetAnimation(const std::string& animationName)
{
	if (CurrentTexture.empty())
	{
		Logger::Error("SpriteRenderer: no texture set before animation " + animationName);
		return false;
	}

	const TextureData& textureData = gData.TextureMgr->GetTextureData(CurrentTexture);
	auto it = textureData.AnimationsData.find(animationName);
	if (it == textureData.AnimationsData.end())
	{
		Logger::Error("SpriteRenderer: unknown animation " + animationName + " in texture " + CurrentTexture);
		return false;
	}

	CurrentAnimationData = it->second;
	CurrentAnimation = animationName;
	Reset();
	ApplyFrame();
	return true;
}

bool SpriteRenderer::SetTextureAndAnimation(const std::string& textureName, const std::string& animationName)
{
	return SetTexture(textureName) && SetAnimation(animationName);
}

void SpriteRenderer::SetTextureRect(const sf::IntRect& rect)
{
	if (Sprite)
		Sprite->setTextureRect(rect);
}

void SpriteRenderer::EnableAnimation(bool enable)
{
	PlayAnimation = enable;
}

bool SpriteRenderer::IsAnimationEnabled() const
{
	return PlayAnimation;
}

void SpriteRenderer::Reset()
{
	CurrentAnimationTime = 0.f;
	CurrentAnimationNb = 0;
}

sf::Vector2f SpriteRenderer::GetSize() const
{
	if (!Sprite)
		return sf::Vector2f(0.f, 0.f);

	return Sprite->getGlobalBounds().size;
}

const std::string& SpriteRenderer::GetCurrentTexture() const
{
	return CurrentTexture;
}

const std::string& SpriteRenderer::GetCurrentAnimation() const
{
	return CurrentAnimation;
}

int SpriteRenderer::GetCurrentFrame() const
{
	return CurrentAnimationNb;
}

void SpriteRenderer::ApplyFrame()
{
	if (!Sprite || CurrentAnimationData.SpriteOnLine <= 0 || CurrentAnimationData.AnimationSpriteCount <= 0)
		return;

	int line = CurrentAnimationNb / CurrentAnimationData.SpriteOnLine;
	int column = CurrentAnimationNb % CurrentAnimationData.SpriteOnLine;

	sf::IntRect rect;
	rect.size.y = CurrentAnimationData.SizeY;
	rect.position.y = CurrentAnimationData.StartY + line * (CurrentAnimationData.OffsetY + CurrentAnimationData.SizeY);

	if (!CurrentAnimationData.IsReverted)
	{
		rect.size.x = CurrentAnimationData.SizeX;
		rect.position.x = CurrentAnimationData.StartX + column * (CurrentAnimationData.OffsetX + CurrentAnimationData.SizeX);
	}
	else
	{
		rect.size.x = -CurrentAnimationData.SizeX;
		rect.position.x = CurrentAnimationData.StartX + column * (CurrentAnimationData.OffsetX + CurrentAnimationData.SizeX) + CurrentAnimationData.SizeX;
	}

	Sprite->setTextureRect(rect);
}
