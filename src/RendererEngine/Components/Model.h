#pragma once

#include "Material.h"
#include "Mesh.h"
#include "Transform.h"

namespace RenderingEngine
{
    class Model
    {
    public:
        Model();

        const uint32_t GetID() const { return m_UID; }
        Transform* GetTransform() const { return m_Transform.get(); }

        std::string Name = "Model";
        Ref<Material> Material;
        Ref<Mesh> Mesh;

    private:
        void DefineUID();

        Scope<Transform> m_Transform;

        uint32_t m_UID = 0;
    };
}
