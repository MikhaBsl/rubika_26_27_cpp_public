#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <filesystem>
#include <string>
#include <DebugMgr.h>
#include <map>

struct AnimationData
{
	int StartX = 0;
	int StartY = 0;
	int SizeX = 0;
	int SizeY = 0;
	int OffsetX = 0;
	int OffsetY = 0;
	int AnimationSpriteCount = 0;
	int SpriteOnLine = 0;
	bool IsReverted = false;
	float TimeBetweenAnimationInS = 0.f;
};

struct StaticTileData
{
	int StartX = 0;
	int StartY = 0;
	int SizeX = 0;
	int SizeY = 0;
	bool IsRevertedX = false;
	bool IsRevertedY = false;
};

struct TextureData
{
	TextureData();
	~TextureData();

	sf::Texture Texture;
	// Container of AnimationData AnimationsData;
	// Container of StaticTileData StaticTilesData;

	friend class TextureMgr;
};

class TextureMgr final : public IDebugable
{
public:
	void Init();
	void Shut();

	TextureMgr();
	~TextureMgr();

	std::map<std::string, TextureData> textures;
	TextureData defaultTexture;

	int LoadFolder(const std::filesystem::path& folder);

	bool LoadTexture(const std::filesystem::path& path);

	/*
	 * Loads the texture using the given path, including its metadata.
	 * Obviously, this function will also store the loaded data
	 * If errors occur during the loading process, this function must return false.
	 * Return true otherwise.
	*/


	/*
	 * Returns the corresponding TextureData using the name of the texture. Returns the default TextureData in case of an unknown name
	 */
	const TextureData& GetTextureData(const std::string& name) const;
	

	// Hérité via IDebugable
	void DrawDebug() override;

private:
	char rootFolder[256] = "Ressources";

	bool CheckFilesExist(const std::filesystem::path& texturePath,
		const std::filesystem::path& metadataPath) const;

	bool LoadImageFile(const std::filesystem::path& path, TextureData& data);
};