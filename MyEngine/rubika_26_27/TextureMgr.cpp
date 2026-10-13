#include "TextureMgr.h"
#include <Globals.h>

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

bool TextureMgr::LoadTexture(const std::filesystem::path& path)
{
	return false;
}

const TextureData& TextureMgr::GetTextureData(const std::string& name) const
{
	auto it = textures.find(name);
	if (it == textures.end()) return defaultTexture;
	return it->second;
}

void TextureMgr::DrawDebug()
{
}

TextureData::TextureData()
{
}

TextureData::~TextureData()
{
}
