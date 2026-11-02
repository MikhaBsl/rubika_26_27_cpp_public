#pragma once
#include <IDrawable.h>
#include <TextureMgr.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <string>

class RectangleRenderer : public IDrawable
{
public:

	RectangleRenderer();
	~RectangleRenderer();

	// Hérité via IDrawable
	void Start() override;
	void Update(float deltaTime) override;
	void Destroy() override;

	bool SetTexture(const std::string& textureName);
	bool SetStaticTile(const std::string& staticTileName);
	bool SetTextureAndStaticTile(const std::string& textureName, const std::string& staticTileName);

	void SetFillColor(const sf::Color& color);
	sf::Color GetFillColor() const;
	sf::Vector2f GetSize() const;

	const std::string& GetCurrentTexture() const;
	const std::string& GetCurrentTile() const;

protected:
	void ApplyTile();

	sf::RectangleShape Shape;
	std::string CurrentTexture;
	std::string CurrentTile;

	StaticTileData TileData;
};