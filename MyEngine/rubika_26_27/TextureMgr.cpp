#include "TextureMgr.h"
#include <Globals.h>
#include <Logger.h>
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>
#include <rapidxml/rapidxml_utils.hpp>
#include <cstdlib>

static int ReadInt(rapidxml::xml_node<>* parent, const char* childName)
{
	rapidxml::xml_node<>* node = parent->first_node(childName);
	if (node == nullptr)
	{
		Logger::Warning(std::string("Missing node: ") + childName);
		return 0;
	}
	return std::atoi(node->value());
}

static void ParseAnimations(rapidxml::xml_node<>* root, TextureData& data)
{
	for (auto* node = root->first_node("Animation"); node != nullptr; node = node->next_sibling("Animation"))
	{
		rapidxml::xml_attribute<>* nameAttr = node->first_attribute("Name");
		if (nameAttr == nullptr)
		{
			Logger::Warning("Animation without Name, skipped");
			continue;
		}

		const std::string name = nameAttr->value();
		if (data.AnimationsData.contains(name))
		{
			Logger::Warning("Duplicate animation name: " + name);
			continue;
		}

		AnimationData anim;
		anim.StartX = ReadInt(node, "X");
		anim.StartY = ReadInt(node, "Y");
		anim.SizeX = ReadInt(node, "SizeX");
		anim.SizeY = ReadInt(node, "SizeY");
		anim.OffsetX = ReadInt(node, "OffsetX");
		anim.OffsetY = ReadInt(node, "OffsetY");
		anim.AnimationSpriteCount = ReadInt(node, "SpriteNum");
		anim.SpriteOnLine = ReadInt(node, "SpritesOnLine");
		anim.IsReverted = ReadInt(node, "Reverted") != 0;

		data.AnimationsData[name] = anim;
	}
}

static void ParseTiles(rapidxml::xml_node<>* root, TextureData& data)
{
	for (auto* node = root->first_node("Tile"); node != nullptr; node = node->next_sibling("Tile"))
	{
		rapidxml::xml_attribute<>* nameAttr = node->first_attribute("Name");
		if (nameAttr == nullptr)
		{
			Logger::Warning("Tile without Name, skipped");
			continue;
		}

		const std::string name = nameAttr->value();
		if (data.StaticTilesData.contains(name))
		{
			Logger::Warning("Duplicate tile name: " + name);
			continue;
		}

		StaticTileData tile;
		tile.StartX = ReadInt(node, "X");
		tile.StartY = ReadInt(node, "Y");
		tile.SizeX = ReadInt(node, "SizeX");
		tile.SizeY = ReadInt(node, "SizeY");
		tile.IsRevertedX = ReadInt(node, "RevertedX") != 0;
		tile.IsRevertedY = ReadInt(node, "RevertedY") != 0;

		data.StaticTilesData[name] = tile;
	}
}

void TextureMgr::Init()
{
	gData.DebugMgr->RegisterDebugableWindow("Texture Manager", this);
}

void TextureMgr::Shut()
{
	gData.DebugMgr->UnregisterDebugableWindow("Texture Manager");
}

TextureMgr::TextureMgr()
{
}

TextureMgr::~TextureMgr()
{
}

const std::map<std::string, TextureData>& TextureMgr::GetTextures() const
{
	return textures;
}

int TextureMgr::LoadFolder(const std::filesystem::path& folder)
{
	if (!std::filesystem::is_directory(folder))
	{
		Logger::Error("Folder not found: " + folder.string());
		return 0;
	}

	int loadedCount = 0;

	for (const auto& entry : std::filesystem::directory_iterator(folder))
	{
		if (!entry.is_regular_file() || entry.path().extension() != ".png")
			continue;

		if (LoadTexture(entry.path()))
			++loadedCount;
	}

	Logger::Info("Loaded " + std::to_string(loadedCount) + " texture(s) from " + folder.string());
	return loadedCount;
}

bool TextureMgr::LoadTexture(const std::filesystem::path& path)
{
	const std::string name = path.stem().string();
	if (textures.contains(name))
	{
		Logger::Warning("Texture already loaded: " + name);
		return true;
	}

	std::filesystem::path metadataPath = path;
	metadataPath.replace_extension(".xml");

	if (!CheckFilesExist(path, metadataPath))
		return false;

	TextureData data;
	if (!LoadImageFile(path, data))
		return false;

	if (!LoadMetadata(metadataPath, data))
		return false;

	textures[name] = std::move(data);
	return true;
}

bool TextureMgr::LoadImageFile(const std::filesystem::path& path, TextureData& data)
{
	if (!data.Texture.loadFromFile(path.string()))
	{
		Logger::Error("Failed to load texture: " + path.string());
		return false;
	}

	Logger::Info("Texture loaded: " + path.string());
	return true;
}

bool TextureMgr::LoadMetadata(const std::filesystem::path& metadataPath, TextureData& data)
{
	try
	{
		rapidxml::file<> file(metadataPath.string().c_str());
		rapidxml::xml_document<> doc;
		doc.parse<0>(file.data());

		rapidxml::xml_node<>* root = doc.first_node();
		if (root == nullptr)
		{
			Logger::Error("Metadata file has no root node: " + metadataPath.string());
			return false;
		}

		const std::string rootName = root->name();
		if (rootName == "Animations")
		{
			data.Type = eTextureType::Animations;
			ParseAnimations(root, data);
			Logger::Info("Animations parsed: " + std::to_string(data.AnimationsData.size()));
		}
		else if (rootName == "Backgrounds")
		{
			data.Type = eTextureType::Backgrounds;
			ParseTiles(root, data);
			Logger::Info("Tiles parsed: " + std::to_string(data.StaticTilesData.size()));
		}
		else
		{
			Logger::Error("Unknown root node '" + rootName + "' in " + metadataPath.string());
			return false;
		}

		return true;
	}
	catch (const std::exception& e)
	{
		Logger::Error("Failed to parse " + metadataPath.string() + ": " + e.what());
		return false;
	}
}

const TextureData& TextureMgr::GetTextureData(const std::string& name) const
{
	auto it = textures.find(name);
	if (it == textures.end()) return defaultTexture;
	return it->second;
}

bool TextureMgr::CheckFilesExist(const std::filesystem::path& texturePath,
	const std::filesystem::path& metadataPath) const
{
	if (!std::filesystem::exists(texturePath))
	{
		Logger::Error("Texture file not found: " + texturePath.string());
		return false;
	}

	if (!std::filesystem::exists(metadataPath))
	{
		Logger::Error("Metadata file not found: " + metadataPath.string());
		return false;
	}

	return true;
}

#ifdef _USE_IMGUI
static void DrawTextureNode(const std::string& name, const TextureData& data)
{
	if (!ImGui::TreeNode(name.c_str()))
		return;

	sf::Vector2u size = data.Texture.getSize();
	ImGui::Text("Size: %u x %u", size.x, size.y);

	if (size.x > 0)
	{
		float maxWidth = ImGui::GetContentRegionAvail().x;
		float width = std::min(static_cast<float>(size.x), maxWidth);
		float height = width * size.y / size.x;

		ImGui::Image(data.Texture, sf::Vector2f(width, height));

	}

	if (data.Type == eTextureType::Animations)
	{
		ImGui::Text("Animations: %zu", data.AnimationsData.size());
		for (const auto& [animName, anim] : data.AnimationsData)
			ImGui::BulletText("%s (%dx%d, %d sprites)", animName.c_str(), anim.SizeX, anim.SizeY, anim.AnimationSpriteCount);
	}
	else if (data.Type == eTextureType::Backgrounds)
	{
		ImGui::Text("Tiles: %zu", data.StaticTilesData.size());
		for (const auto& [tileName, tile] : data.StaticTilesData)
			ImGui::BulletText("%s (%dx%d)", tileName.c_str(), tile.SizeX, tile.SizeY);
	}

	ImGui::TreePop();
}
#endif

void TextureMgr::DrawDebug()
{
#ifdef _USE_IMGUI
	ImGui::InputText("Root folder", rootFolder, sizeof(rootFolder));
	ImGui::SameLine();
	if (ImGui::Button("Load folder"))
		LoadFolder(rootFolder);

	ImGui::Text("Loaded textures: %zu", textures.size());
	ImGui::Separator();

	if (ImGui::BeginTabBar("TextureTabs"))
	{
		if (ImGui::BeginTabItem("Animations"))
		{
			for (const auto& [name, data] : textures)
				if (data.Type == eTextureType::Animations)
					DrawTextureNode(name, data);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Backgrounds"))
		{
			for (const auto& [name, data] : textures)
				if (data.Type == eTextureType::Backgrounds)
					DrawTextureNode(name, data);
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
#endif
}

TextureData::TextureData()
{
}

TextureData::~TextureData()
{
}