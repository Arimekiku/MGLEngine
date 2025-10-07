#include "Skybox.h"

namespace Editor
{
	Skybox::Skybox()
	{
		m_Mesh = MeshImporter::CreateCube();

		m_Transform = Ptr<TransformComponent>::Create();
		m_Transform->Scale = glm::vec3(100.0f);

		m_Cubemap = Ptr<Texture>::Create(
			RESOURCES_PATH "Images/Cubemap/nx.png",
			RESOURCES_PATH "Images/Cubemap/px.png",
			RESOURCES_PATH "Images/Cubemap/ny.png",
			RESOURCES_PATH "Images/Cubemap/py.png",
			RESOURCES_PATH "Images/Cubemap/nz.png",
			RESOURCES_PATH "Images/Cubemap/pz.png"
		);

		const auto shader = Ptr<Shader>::Create(RESOURCES_PATH "Shaders/skybox.vert", RESOURCES_PATH "Shaders/skybox.frag");
		shader->Bind();
		shader->BindUniformInt1("u_Skybox", 0);
		shader->Unbind();

		m_Mat = Ptr<Material>::Create(shader);
		m_Mat->AddTexture(m_Cubemap);
		m_Mat->SetCullMode(Front);
	}
}
