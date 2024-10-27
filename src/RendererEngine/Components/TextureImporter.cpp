#include "TextureImporter.h"

namespace RenderingEngine
{
    static std::unordered_map<std::string, Ref<Texture>> m_Textures;

    const Ref<Texture>& TextureImporter::GetTexture(const std::string& path)
    {
        if (m_Textures[path])
        {
            return m_Textures[path];
        }

        std::stringstream resultPath;
        resultPath << RESOURCES_PATH << path;

        m_Textures[path] = std::make_shared<Texture>(resultPath.str());
        return m_Textures[path];
    }
}