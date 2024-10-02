#include "mxpch.h"
#include "Model.h"

#include "RendererEngine/Core/Logger/Log.h"
#include "MeshImporter.h"

#include <random>

namespace RenderingEngine
{
    static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

    Model::Model(const Ref<Mesh>& mesh, const Ref<Material>& material)
    {
        m_Mesh = mesh;
        m_Material = material;

        DefineUID();
    }

    Model::Model(const Ref<Material>& material)
    {
        m_Mesh = std::make_shared<Mesh>();
        m_Material = material;

        DefineUID();
    }

    void Model::DefineUID()
    {
        m_UID = s_UniformDistribution(s_Engine);
    }
}
