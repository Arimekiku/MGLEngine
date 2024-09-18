#include "mxpch.h"
#include "Model.h"

namespace RenderingEngine
{
    Model::Model(const Ref<Mesh>& mesh, const Ref<Shader>& shader)
    {
        m_Mesh = mesh;
        m_Shader = shader;
        m_Transform = std::make_shared<Transform>();
    }

    Model::Model(const char* path)
    {
        //TODO make or import some assimp
    }
}
