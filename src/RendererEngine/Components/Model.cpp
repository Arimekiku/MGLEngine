#include "mxpch.h"
#include "Model.h"

#include "RendererEngine/Core/Logger/Log.h"
#include "MeshImporter.h"

namespace RenderingEngine
{
    Model::Model(const Ref<Mesh>& mesh, const Ref<Material>& material)
    {
        m_Mesh = mesh;
        m_Material = material;
    }

    Model::Model(const Ref<Material>& material)
    {
        m_Mesh = std::make_shared<Mesh>();
        m_Material = material;
    }

    Model::Model(const char* path)
    {
        m_Mesh = MeshImporter::CreateMesh(path);
    }
}
