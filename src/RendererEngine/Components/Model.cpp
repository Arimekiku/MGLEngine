#include "mxpch.h"
#include "Model.h"

namespace RenderingEngine
{
    Model::Model(const Ref<Mesh>& mesh, const Ref<Material>& material)
    {
        m_Mesh = mesh;
        m_Material = material;
        m_Transform = std::make_shared<Transform>();
    }

    Model::Model(const Ref<Material>& material)
    {
        m_Mesh = std::make_shared<Mesh>();
        m_Material = material;
        m_Transform = std::make_shared<Transform>();
    }

    Model::Model(const char* path)
    {
        //TODO make or import some assimp
    }
}
