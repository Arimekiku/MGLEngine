#include "MeshImporter.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace RenderingEngine
{
    static std::unordered_map<std::string, Ref<Mesh>> m_Meshes;

    const Ref<Mesh>& MeshImporter::CreateMesh(const std::string& path)
    {
        if (m_Meshes[path])
        {
            return m_Meshes[path];
        }

        Assimp::Importer importer;

        std::stringstream resultPath;
        resultPath << RESOURCES_PATH << path;
        const aiScene* scene = importer.ReadFile(resultPath.str().c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG_ASSERT(false, "Could not load from path: {0} with message: {1}", resultPath.str().c_str(), importer.GetErrorString());
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

            Vertex vertex{ position, normal, texCoord };
            vertices.push_back(vertex);
        }

        for (uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for (uint32_t j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        m_Meshes[path] = std::make_shared<Mesh>(vertices, indices);
        m_Meshes[path]->SetPath(path);
        return m_Meshes[path];
    }

    const Ref<Mesh>& MeshImporter::CreatePlane()
    {
        if (m_Meshes["Plane"])
        {
            return m_Meshes["Plane"];
        }

        auto vertices = std::vector<Vertex>(4);
        vertices[0].Position = glm::vec3(-0.5f, 0.5f, 0.0f);
        vertices[0].TexCoord = glm::vec2(1.0f, 0.0f);

        vertices[1].Position = glm::vec3(-0.5f, -0.5f, 0.0f);
        vertices[1].TexCoord = glm::vec2(1.0f, 1.0f);

        vertices[2].Position = glm::vec3(0.5f, -0.5f, 0.0f);
        vertices[2].TexCoord = glm::vec2(0.0f, 1.0f);

        vertices[3].Position = glm::vec3(0.5f, 0.5f, 0.0f);
        vertices[3].TexCoord = glm::vec2(0.0f, 0.0f);

        for (auto& vertex : vertices)
        {
            vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
        }

        std::vector<uint32_t> indices = { 0, 1, 2, 0, 2, 3, };

        m_Meshes["Plane"] = std::make_shared<Mesh>(vertices, indices);
        m_Meshes["Plane"]->SetPath("Plane");
        return m_Meshes["Plane"];
    }

    const Ref<Mesh>& MeshImporter::CreateCube()
    {
        if (m_Meshes["Cube"])
        {
            return m_Meshes["Cube"];
        }

        auto vertices = std::vector<Vertex>(8);
        vertices[0].Position = glm::vec3(-0.5f, -0.5f, 0.5f);
        vertices[1].Position = glm::vec3( 0.5f, -0.5f, 0.5f);
        vertices[2].Position = glm::vec3( 0.5f,  0.5f, 0.5f);
        vertices[3].Position = glm::vec3(-0.5f,  0.5f, 0.5f);
        vertices[4].Position = glm::vec3(-0.5f, -0.5f, -0.5f);
        vertices[5].Position = glm::vec3( 0.5f, -0.5f, -0.5f);
        vertices[6].Position = glm::vec3( 0.5f,  0.5f, -0.5f);
        vertices[7].Position = glm::vec3(-0.5f,  0.5f, -0.5f);

        vertices[0].Normal = glm::vec3(-1.0f, -1.0f,  1.0f);
		vertices[1].Normal = glm::vec3( 1.0f, -1.0f,  1.0f);
		vertices[2].Normal = glm::vec3( 1.0f,  1.0f,  1.0f);
		vertices[3].Normal = glm::vec3(-1.0f,  1.0f,  1.0f);
		vertices[4].Normal = glm::vec3(-1.0f, -1.0f, -1.0f);
		vertices[5].Normal = glm::vec3( 1.0f, -1.0f, -1.0f);
		vertices[6].Normal = glm::vec3( 1.0f,  1.0f, -1.0f);
		vertices[7].Normal = glm::vec3(-1.0f,  1.0f, -1.0f);

        std::vector<uint32_t> indices =
        {
            0, 1, 2, 2, 3, 0,
            1, 5, 6, 6, 2, 1,
            7, 6, 5, 5, 4, 7,
            4, 0, 3, 3, 7, 4,
            4, 5, 1, 1, 0, 4,
            3, 2, 6, 6, 7, 3,
        };
        
        m_Meshes["Cube"] = std::make_shared<Mesh>(vertices, indices);
        m_Meshes["Cube"]->SetPath("Cube");
        return m_Meshes["Cube"];
    }

    const Ref<Mesh>& MeshImporter::CreateSphere()
    {
        if (m_Meshes["Sphere"])
        {
            return m_Meshes["Sphere"];
        }

        const float radius = 1.0f;
        const float pi = 3.14f;
		constexpr float latitudeBands = 30;
		constexpr float longitudeBands = 30;

        std::vector<Vertex> vertices;
		for (float latitude = 0.0f; latitude <= latitudeBands; latitude++)
		{
			const float theta = latitude * (float)pi / latitudeBands;
			const float sinTheta = glm::sin(theta);
			const float cosTheta = glm::cos(theta);

			for (float longitude = 0.0f; longitude <= longitudeBands; longitude++)
			{
				const float phi = longitude * 2.f * (float)pi / longitudeBands;
				const float sinPhi = glm::sin(phi);
				const float cosPhi = glm::cos(phi);

                auto vertex = Vertex();
				vertex.Normal = -glm::vec3(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
				vertex.Position = glm::vec3(radius * vertex.Normal.x, radius * vertex.Normal.y, radius * vertex.Normal.z);
				vertices.push_back(vertex);
			}
		}

		std::vector<uint32_t> indices;
		for (uint32_t latitude = 0; latitude < latitudeBands; latitude++)
		{
			for (uint32_t longitude = 0; longitude < longitudeBands; longitude++)
			{
				const uint32_t first = (latitude * (longitudeBands + 1)) + longitude;
				const uint32_t second = first + longitudeBands + 1;

				indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
			}
		}

        m_Meshes["Sphere"] = std::make_shared<Mesh>(vertices, indices);
        m_Meshes["Sphere"]->SetPath("Sphere");
        return m_Meshes["Sphere"];
    }
}