#include "mxpch.h"
#include "Scene.h"

#include <imgui.h>

namespace RenderingEngine
{
	Scene::Scene()
	{
		const auto& shader = std::make_shared<Shader>(
            RESOURCES_PATH "Shaders/default.vert",
            RESOURCES_PATH "Shaders/default.frag");

        const auto& m_FaceTexture = std::make_shared<Texture>(RESOURCES_PATH "Images/face.png");
        const auto& m_HouseTexture = std::make_shared<Texture>(RESOURCES_PATH "Images/house.png");

        m_Light = std::make_shared<AreaLighting>(glm::vec3(0, 4, 4));
        m_Light->Color = glm::vec3(300, 300, 300);

        shader->Bind();
        shader->BindUniformInt1("u_Texture", 0);
        shader->BindUniformFloat3("u_LightColor", m_Light->Color);
        shader->BindUniformFloat3("u_LightPos", m_Light->GetTransform()->Position);

        m_DefaultMaterial = std::make_shared<Material>(shader);
        m_DefaultMaterial->SetTextureMap(m_HouseTexture);
	}

	Scene::~Scene()
	{

	}

	void Scene::OnEveryUpdate()
	{
		for (auto& model : m_Instances)
		{
			Renderer::RenderModel(model);
		}
	}

	void Scene::OnGUIUpdate()
	{
		ImGui::Begin("Scene");

		for (auto& model : m_Instances)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= (m_SelectedEntity == model) ? ImGuiTreeNodeFlags_Selected : 0;
			bool isOpened = ImGui::TreeNodeEx((void*)model->GetID(), flags, model->GetName());

			if (ImGui::IsItemClicked(0))
			{
				m_SelectedEntity = model;
			}

			if (isOpened)
			{
				ImGui::TreePop();
			}
		}

		ImGui::End();

		ImGui::Begin("Inspector");

		if (m_SelectedEntity)
		{
			char buf[256];
			memset(buf, 0, 256);
			strcpy(buf, m_SelectedEntity->GetName());

			if (ImGui::InputText("Name", buf, sizeof(buf)))
			{
				m_SelectedEntity->SetName(buf);
			}

			if (ImGui::TreeNodeEx((void*)typeid(Transform).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				ImGui::DragFloat3("Position", glm::value_ptr(m_SelectedEntity->GetPosition()));
				ImGui::DragFloat3("Rotation", glm::value_ptr(m_SelectedEntity->GetRotation()));
				ImGui::DragFloat3("Scale", glm::value_ptr(m_SelectedEntity->GetScale()));

				ImGui::TreePop();
			}
		}

		ImGui::End();
	}

	const Ref<Model>& Scene::Instantiate(const Ref<Mesh>& mesh, const glm::vec3 transform)
	{
		auto model = std::make_shared<Model>(mesh, m_DefaultMaterial);

		auto& position = model->GetPosition();
		position = transform;
		
		m_Instances.push_back(model);

		return model;
	}
}