#include "MaterialImporter.h"
#include "RendererEngine/Core/Logger/Log.h"

namespace RenderingEngine
{
    static std::unordered_map<std::string, Ref<Material>> m_Materials;

    const Ref<Material>& MaterialImporter::GetMaterial(const std::string& name)
    {
        if (m_Materials[name])
        {
            return m_Materials[name];
        }

        LOG_EDITOR_ERROR("Material {0} do not exists!", name);
        throw std::exception();
    }

    const Ref<Material>& MaterialImporter::AddMaterial(const std::string& name, const std::string& vertPath, const std::string& fragPath)
    {
        if (m_Materials[name])
        {
            return m_Materials[name];
        }

        std::stringstream vertResult;
        vertResult << RESOURCES_PATH << vertPath;

        std::stringstream fragResult;
        fragResult << RESOURCES_PATH << fragPath;

        m_Materials[name] = std::make_shared<Material>(vertPath, fragPath);
        m_Materials[name]->SetName(name);
        return m_Materials[name];
    }
}