#pragma once

#include "RendererEngine/Core.h"
#include "RendererEngine/Components/Texture.h"

namespace RenderingEngine
{
	class TextureImporter
	{
	public:
		static const Ref<Texture>& GetTexture(const std::string& path);
	};
}