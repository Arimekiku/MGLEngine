#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"

namespace RenderingEngine
{
    class Model
    {
    public:
        explicit Model(const Ref<Mesh>& mesh, const Ref<Shader>& shader);
        explicit Model(const char* path);

        Ref<Mesh> GetMesh() { return m_Mesh; }
        Ref<Shader> GetShader() { return m_Shader; }
        Ref<Transform> GetTransform() { return m_Transform; }

    private:
        Ref<Mesh> m_Mesh;
        Ref<Shader> m_Shader;
        Ref<Transform> m_Transform;
    };
}
