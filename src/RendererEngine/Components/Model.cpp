#include "mxpch.h"
#include "Model.h"

#include "RendererEngine/Core/Logger/Log.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

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
        m_Mesh = std::make_shared<Mesh>();

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
	        LOG_CORE_ASSERT(false, "Could not load from path: {0} with message: {1}", path, importer.GetErrorString());
            return;
        }

        aiMesh* mesh = scene->mMeshes[0];

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        for (int i = 0; i < mesh->mNumVertices; i++)
        {
            auto normal = glm::vec3(0);
            if (mesh->HasNormals())
            {
                normal.x = mesh->mNormals[i].x;
                normal.x = mesh->mNormals[i].y;
                normal.x = mesh->mNormals[i].z;
            }

            auto texCoord = glm::vec2(0);
            if (mesh->mTextureCoords[0])
            {
                texCoord.x = mesh->mTextureCoords[0][i].x;
                texCoord.x = mesh->mTextureCoords[0][i].y;
            }

            const auto position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

            Vertex vertex { position, normal, texCoord };
            vertices.push_back(vertex);
        }

        for(uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for(uint32_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        m_Mesh->SetVertices(vertices);
        m_Mesh->SetIndices(indices);
    }
}
