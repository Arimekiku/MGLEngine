#include "Scene.h"
#include "GuiRenderer.h"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

namespace RenderingEngine
{
	Scene::Scene() : m_Camera(glm::vec3(0, 0, 10))
	{
		const auto& lightShader = std::make_shared<Shader>(
			RESOURCES_PATH "Shaders/areaLight.vert",
            RESOURCES_PATH "Shaders/areaLight.frag");

		lightShader->Bind();
		lightShader->BindUniformFloat3("u_LightColor", {1, 1, 1});

		const auto& lightMaterial = std::make_shared<Material>(lightShader);

		const auto& lightModel = std::make_shared<Model>(MeshImporter::CreateCube(), lightMaterial);
		m_Light = AreaLighting(glm::vec3(0, 5, -5), lightModel);

        const auto& m_FaceTexture = std::make_shared<Texture>(RESOURCES_PATH "Images/face.png");
        const auto& m_HouseTexture = std::make_shared<Texture>(RESOURCES_PATH "Images/house.png");

		m_ShadowMapShader = std::make_shared<Shader>(
			RESOURCES_PATH "Shaders/shadowMap.vert",
            RESOURCES_PATH "Shaders/shadowMap.frag");

		m_ShadowMapShader->Bind();
		m_ShadowMapShader->BindUniformMat4("u_lightViewProj", m_DirLight.GetProjViewMat());

		const auto& shader = std::make_shared<Shader>(
            RESOURCES_PATH "Shaders/default.vert",
            RESOURCES_PATH "Shaders/default.frag");

        shader->Bind();
        shader->BindUniformInt1("u_Texture", 0);
		shader->BindUniformInt1("u_DepthMap", 1);
        shader->BindUniformFloat3("u_LightColor", m_Light.Color);
        shader->BindUniformFloat3("u_LightPos", m_Light.Position);

        m_DefaultMaterial = std::make_shared<Material>(shader);
        m_DefaultMaterial->SetTextureMap(m_HouseTexture);
	}

	const Ref<Model>& Scene::Instantiate(const Ref<Mesh>& mesh, const glm::vec3 transform)
	{
		auto model = std::make_shared<Model>(mesh, m_DefaultMaterial);

		auto& position = model->GetPosition();
		position = transform;
		
		m_Instances.push_back(model);
		return m_Instances.back();
	}

	void Scene::OnEveryUpdate(Time deltaTime)
	{
		Renderer::OnEveryUpdate(m_Camera);

        m_Camera.EveryUpdate(deltaTime);

		glCullFace(GL_FRONT);
		m_DepthMap.Bind();

		//Draw call for shadow map
        Renderer::Clear(glm::vec4(0, 0, 0, 1));
		for (auto& model : m_Instances)
		{
			Renderer::RenderMesh(model->GetMesh(), m_ShadowMapShader, model->GetTRSMatrix());
		}

		Framebuffer::Unbind();
		glCullFace(GL_BACK);

		m_Viewport.Bind();

		//Draw call for actual models
        Renderer::Clear(glm::vec4(0, 0, 0, 1));

		m_Light.OnEveryUpdate();

		for (auto& model : m_Instances)
		{
			m_DefaultMaterial->GetShader()->Bind();
			glBindTextureUnit(0, m_DefaultMaterial->GetProperties().AlbedoID);
			glBindTextureUnit(1, m_DepthMap.GetAttachment(0));

			m_DefaultMaterial->GetShader()->BindUniformMat4("u_lightViewProj", m_DirLight.GetProjViewMat());

			Renderer::RenderModel(model);
		}

		Framebuffer::Unbind();
	}

	void Scene::OnGUIUpdate()
	{
		DrawScenePanel();

		DrawInspectorPanel();

		DrawViewport();
	}

	void Scene::DrawScenePanel()
	{
		ImGui::Begin("Scene");

		for (auto& model : m_Instances)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= (m_SelectedEntity == model) ? ImGuiTreeNodeFlags_Selected : 0;
			bool isOpened = ImGui::TreeNodeEx((void*)model->GetID(), flags, "%s", model->GetName());

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

	void Scene::DrawViewport() 
	{
		ImGui::Begin("Viewport");

        const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        const auto castSize = glm::i16vec2(viewportSize.x, viewportSize.y);

        if (castSize.x != 0 && castSize.y != 0)
        {
            if (glm::i16vec2(m_Viewport.GetWidth(), m_Viewport.GetHeight()) != castSize)
            {
                GetSceneCamera().Resize(castSize.x, castSize.y);
                m_Viewport.Resize(castSize.x, castSize.y);
				m_DepthMap.Resize(castSize.x, castSize.y);
            }

            const uint32_t m_Texture = m_Viewport.GetAttachment(0);
            ImGui::Image(m_Texture, ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 0), ImVec2(1, -1));
        }

		DrawGuizmos();

        ImGui::End();
	}

	void Scene::DrawGuizmos() 
	{
		if (!m_SelectedEntity) 
		{
			return;
		}

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		float width = ImGui::GetWindowWidth();
		float height = ImGui::GetWindowWidth();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, width, height);

		glm::mat4 cameraView = m_Camera.GetViewMat();
		glm::mat4 cameraProj = m_Camera.GetProjMat();
		glm::mat4 entityTRS = m_SelectedEntity->GetTRSMatrix();

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProj), 
		ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(entityTRS));

		if (ImGuizmo::IsUsing()) 
		{
			auto& position = m_SelectedEntity->GetPosition();
			position = glm::vec3(entityTRS[3]);
		}
	}
}