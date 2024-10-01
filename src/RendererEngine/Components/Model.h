#pragma once

#include "Material.h"
#include "Mesh.h"
#include "Transform.h"

namespace RenderingEngine
{
    class Model
    {
    public:
        Model(const Ref<Mesh>& mesh, const Ref<Material>& material);
        explicit Model(const Ref<Material>& material);
        explicit Model(const char* path);

        void SetMesh(const Ref<Mesh>& mesh) { m_Mesh = mesh; }
        void SetMaterial(const Ref<Material>& mat) { m_Material = mat; }

        const Ref<Mesh>& GetMesh() { return m_Mesh; }
        const Ref<Material>& GetMaterial() { return m_Material; }

        [[nodiscard]] glm::mat4 GetTRSMatrix() const { return m_Transform->GetTRSMatrix(); }
        [[nodiscard]] glm::vec3& GetPosition() const { return m_Transform->Position; }
        [[nodiscard]] glm::vec3& GetRotation() const { return m_Transform->Rotation; }
        [[nodiscard]] glm::vec3& GetScale() const { return m_Transform->Scale; }

    private:
        Ref<Mesh> m_Mesh;
        Ref<Material> m_Material;
        Scope<Transform> m_Transform = std::make_unique<Transform>();
    };
}
