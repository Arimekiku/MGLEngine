#include "mxpch.h"
#include "Scene.h"
#include "GuiRenderer.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace RenderingEngine
{
	Scene::Scene() : m_Camera(glm::vec3(0, 0, -2)), m_Light(glm::vec3(0, 4, 4))
	{
		const auto& shader = std::make_shared<Shader>(
            RESOURCES_PATH "Shaders/default.vert",
            RESOURCES_PATH "Shaders/default.frag");

        const auto& m_FaceTexture = std::make_shared<Texture>(RESOURCES_PATH "Images/face.png");
        const auto& m_HouseTexture = std::make_shared<Texture>(RESOURCES_PATH "Images/house.png");

        m_Light.Color = glm::vec3(300, 300, 300);

        shader->Bind();
        shader->BindUniformInt1("u_Texture", 0);
        shader->BindUniformFloat3("u_LightColor", m_Light.Color);
        shader->BindUniformFloat3("u_LightPos", m_Light.Position);

        m_DefaultMaterial = std::make_shared<Material>(shader);
        m_DefaultMaterial->SetTextureMap(m_HouseTexture);
	}

	Scene::~Scene()
	{

	}

	const Ref<Model>& Scene::Instantiate(const Ref<Mesh>& mesh, const glm::vec3 transform)
	{
		auto model = std::make_shared<Model>(mesh, m_DefaultMaterial);

		auto& position = model->GetPosition();
		position = transform;
		
		m_Instances.push_back(model);

		return model;
	}

	void Scene::OnEveryUpdate(Time deltaTime)
	{
		Renderer::OnEveryUpdate(m_Camera);

        m_Camera.EveryUpdate(deltaTime); 

		for (auto& model : m_Instances)
		{
			Renderer::RenderModel(model);
		}
	}

	void Scene::OnGUIUpdate()
	{
		DrawScenePanel();

		DrawInspectorPanel();

		ImGui::ShowDemoWindow();
	}

	void Scene::DrawScenePanel()
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
	}

	void Scene::DrawInspectorPanel()
	{
		ImGui::Begin("Inspector");

		if (m_SelectedEntity)
		{
			GuiRenderer::DrawImGuiModel(m_SelectedEntity);
		}

		ImGui::End();
	}
}