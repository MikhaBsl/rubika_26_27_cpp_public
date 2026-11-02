#include "RectangleRenderer.h"
#include <TextureMgr.h>
#include <Logger.h>
#include <Globals.h>

RectangleRenderer::RectangleRenderer()
{
	Drawable = &Shape;
}

RectangleRenderer::~RectangleRenderer()
{
}

void RectangleRenderer::Start()
{
	ApplyTile();
}

void RectangleRenderer::Update(float deltaTime)
{
}

void RectangleRenderer::Destroy()
{
}

bool RectangleRenderer::SetTexture(const std::string& textureName)
{
	if (!gData.TextureMgr->HasTexture(textureName))
	{
		Logger::Error("RectangleRenderer: unknown texture " + textureName);
		return false;
	}

	const TextureData& textureData = gData.TextureMgr->GetTextureData(textureName);
	Shape.setTexture(&textureData.Texture);

	if (CurrentTexture != textureName)
	{
		CurrentTile.clear();
		TileData = StaticTileData();
		ApplyTile();
	}

	CurrentTexture = textureName;
	return true;
}

bool RectangleRenderer::SetStaticTile(const std::string& staticTileName)
{
	if (CurrentTexture.empty())
	{
		Logger::Error("RectangleRenderer: no texture set before tile " + staticTileName);
		return false;
	}

	const TextureData& textureData = gData.TextureMgr->GetTextureData(CurrentTexture);
	auto it = textureData.StaticTilesData.find(staticTileName);
	if (it == textureData.StaticTilesData.end())
	{
		Logger::Error("RectangleRenderer: unknown tile " + staticTileName + " in texture " + CurrentTexture);
		return false;
	}

	TileData = it->second;
	CurrentTile = staticTileName;
	ApplyTile();
	return true;
}

bool RectangleRenderer::SetTextureAndStaticTile(const std::string& textureName, const std::string& staticTileName)
{
	return SetTexture(textureName) && SetStaticTile(staticTileName);
}

void RectangleRenderer::SetFillColor(const sf::Color& color)
{
	Shape.setFillColor(color);
}

sf::Color RectangleRenderer::GetFillColor() const
{
	return Shape.getFillColor();
}

sf::Vector2f RectangleRenderer::GetSize() const
{
	return Shape.getSize();
}

const std::string& RectangleRenderer::GetCurrentTexture() const
{
	return CurrentTexture;
}

const std::string& RectangleRenderer::GetCurrentTile() const
{
	return CurrentTile;
}

void RectangleRenderer::ApplyTile()
{
	sf::IntRect rect;
	if (!TileData.IsRevertedX)
	{
		rect.size.x = TileData.SizeX;
		rect.position.x = TileData.StartX;
	}
	else
	{
		rect.size.x = -TileData.SizeX;
		rect.position.x = TileData.StartX + TileData.SizeX;
	}

	if (!TileData.IsRevertedY)
	{
		rect.size.y = TileData.SizeY;
		rect.position.y = TileData.StartY;
	}
	else
	{
		rect.size.y = -TileData.SizeY;
		rect.position.y = TileData.StartY + TileData.SizeY;
	}

	Shape.setSize(sf::Vector2f((float)TileData.SizeX, (float)TileData.SizeY));
	Shape.setTextureRect(rect);
}
