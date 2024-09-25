#pragma once

#include "Material.h"
#include "Mesh.h"
#include "Transform.h"

namespace RenderingEngine
{
    class Model
    {
    public:
        explicit Model(const Ref<Mesh>& mesh, const Ref<Material>& material);
        explicit Model(const Ref<Material>& material);
        explicit Model(const char* path);

        Ref<Mesh> GetMesh() { return m_Mesh; }
        Ref<Material> GetMaterial() { return m_Material; }
        Ref<Transform> GetTransform() { return m_Transform; }

    private:
        Ref<Mesh> m_Mesh;
        Ref<Material> m_Material;
        Ref<Transform> m_Transform;
    };
}
