#include "MaterialImporter.h"
#include "Core/Logger/Log.h"

#include <sstream>

namespace Engine
{
    static std::unordered_map<std::string, Ptr<Material>> m_Materials;

    const Ptr<Material>& MaterialImporter::GetMaterial(const std::string& name)
    {
        if (m_Materials[name])
            return m_Materials[name];

        LOG_ERROR("Material {0} do not exists!", name);
        throw std::exception();
    }

    const Ptr<Material>& MaterialImporter::AddMaterial(const std::string& name, const std::string& vertPath, const std::string& fragPath)
    {
        if (m_Materials[name])
        {
            return m_Materials[name];
        }

        std::stringstream vertResult;
        vertResult << RESOURCES_PATH << vertPath;

        std::stringstream fragResult;
        fragResult << RESOURCES_PATH << fragPath;

        m_Materials[name] = Ptr<Material>::Create(vertPath, fragPath);
        m_Materials[name]->SetName(name);
        return m_Materials[name];
    }
}