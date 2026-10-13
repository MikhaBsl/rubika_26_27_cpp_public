#include "TextureMgr.h"
#include <Globals.h>
#include <Logger.h>
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>
#include <rapidxml/rapidxml_utils.hpp>

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
	std::filesystem::path metadataPath = path;
	metadataPath.replace_extension(".xml");

	if (!CheckFilesExist(path, metadataPath))
		return false;

	TextureData data;
	if (!LoadImageFile(path, data))
		return false;

	textures[path.stem().string()] = std::move(data);
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

const TextureData& TextureMgr::GetTextureData(const std::string& name) const
{
	auto it = textures.find(name);
	if (it == textures.end()) return defaultTexture;
	return it->second;
}

void TextureMgr::DrawDebug()
{
#ifdef _USE_IMGUI
	ImGui::InputText("Root folder", rootFolder, sizeof(rootFolder));
	ImGui::SameLine();
	if (ImGui::Button("Load folder"))
		LoadFolder(rootFolder);

	ImGui::Text("Loaded textures: %zu", textures.size());
	ImGui::Separator();

	for (auto& [name, data] : textures)
	{
		if (ImGui::TreeNode(name.c_str()))
		{
			sf::Vector2u size = data.Texture.getSize();
			ImGui::Text("Size: %u x %u", size.x, size.y);

			if (size.x > 0)
			{
				float width = 128.f;
				float height = width * size.y / size.x;
				ImGui::Image(data.Texture, sf::Vector2f(width, height));
			}
			ImGui::TreePop();
		}
	}
#endif
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

TextureData::TextureData()
{
}

TextureData::~TextureData()
{
}
