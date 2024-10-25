#include "GuiRenderer.h"
#include "ComponentRegistry.h"
#include "RendererEngine/Core/Math.h"

namespace RenderingEngine
{
	void GuiRenderer::DrawViewport() 
	{
		ImGui::Begin("Viewport");

        const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        const auto castSize = glm::i16vec2(viewportSize.x, viewportSize.y);

        if (castSize.x != 0 && castSize.y != 0)
        {
            if (glm::i16vec2(m_Context->m_Viewport.GetWidth(), m_Context->m_Viewport.GetHeight()) != castSize)
            {
                //m_Context->GetSceneCamera().Resize(castSize.x, castSize.y);
                m_Context->m_Viewport.Resize(castSize.x, castSize.y);
				m_Context->m_DepthMap.Resize(castSize.x, castSize.y);
            }

            uint32_t m_Texture = m_Context->m_Viewport.GetAttachment(0);
            ImGui::Image((void*)m_Texture, ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 0), ImVec2(1, -1));
        }

		if (m_SelectedEntity)
		{
			DrawGuizmos();
		}

        ImGui::End();
	}

	void GuiRenderer::DrawScenePanel()
	{
		ImGui::Begin("Scene");

		auto& sceneEntities = m_Context->m_Entities.view<NameComponent>();

		for (auto& entity : sceneEntities)
		{
			NameComponent entityName = sceneEntities.get<NameComponent>(entity);

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ((entt::entity)m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0;
			bool isOpened = ImGui::TreeNodeEx((void*)entity, flags, "%s", entityName.Name.c_str());

			if (ImGui::IsItemClicked(0))
			{
				m_SelectedEntity = Entity(entity, m_Context);
			}

			if (isOpened)
			{
				ImGui::TreePop();
			}
		}

		ImGui::End();
	}

	void GuiRenderer::DrawInspectorPanel()
	{
		ImGui::Begin("Inspector");

		if (m_SelectedEntity)
		{
			NameComponent nameComponent;
			if (m_SelectedEntity.TryGetComponent<NameComponent>(nameComponent))
			{
				char buf[256];
				memset(buf, 0, 256);
				strcpy(buf, nameComponent.Name.c_str());

				if (ImGui::InputText("Name", buf, sizeof(buf)))
				{
					nameComponent.Name = buf;
				}
			}

			auto flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

			TransformComponent transformComponent;
			if (m_SelectedEntity.TryGetComponent<TransformComponent>(transformComponent))
			{
				if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), flags, "Transform"))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));

					ImGui::AlignTextToFramePadding();
					DrawVector3Drag("Position", transformComponent.Position, 100.0f);

					ImGui::AlignTextToFramePadding();
					DrawVector3Drag("Rotation", transformComponent.Rotation, 100.0f);

					ImGui::AlignTextToFramePadding();
					DrawVector3Drag("Scale", transformComponent.Scale, 100.0f);

					ImGui::PopStyleVar();
					ImGui::TreePop();
				}
			}

			MaterialComponent materialComponent;
			if (m_SelectedEntity.TryGetComponent<MaterialComponent>(materialComponent))
			{
				if (ImGui::TreeNodeEx((void*)typeid(MaterialComponent).hash_code(), flags, "Matetial"))
				{
					auto& material = materialComponent.SharedMat;
					auto& properties = material->GetProperties();

					ImGui::Image((void*)properties.AlbedoID, ImVec2(64, 64));

					ImGui::ColorEdit3("Albedo", glm::value_ptr(properties.Albedo));
					ImGui::SliderFloat("Rougness", &properties.Roughness, 0, 1, "%.2f");
					ImGui::SliderFloat("Metallic", &properties.Metallic, 0, 1, "%.2f");
					ImGui::SliderFloat("AO", &properties.AO, 0, 1, "%.2f");

					ImGui::TreePop();
				}
			}
		}

		ImGui::End();
	}

	void GuiRenderer::DrawGuizmos() 
	{
		if (!m_SelectedEntity)
		{
			return;
		}

		if (Input::KeyPressed(GLFW_KEY_F1))
			m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

        if (Input::KeyPressed(GLFW_KEY_F2))
			m_GuizmoOperation = ImGuizmo::OPERATION::ROTATE;

        if (Input::KeyPressed(GLFW_KEY_F3))
			m_GuizmoOperation = ImGuizmo::OPERATION::SCALE;

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		float width = ImGui::GetWindowWidth();
		float height = ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, width, height);

		//glm::mat4 cameraView = m_Context->GetSceneCamera().GetViewMat();
		//glm::mat4 cameraProj = m_Context->GetSceneCamera().GetProjMat();

		TransformComponent& entityTransform = m_SelectedEntity.GetComponent<TransformComponent>();
		glm::mat4 entityTRS = entityTransform.GetTRSMatrix();

		//ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProj), 
		//(ImGuizmo::OPERATION)m_GuizmoOperation, ImGuizmo::LOCAL, glm::value_ptr(entityTRS));

		if (ImGuizmo::IsUsing()) 
		{
			glm::vec3 position, rotation, scale;

			if (Math::Decompose(entityTRS, position, rotation, scale) == false) 
			{
				return;
			}

			entityTransform.Position = position;

			glm::vec3 deltaRot = rotation - glm::radians(entityTransform.Rotation);
			entityTransform.Rotation += glm::degrees(deltaRot);

			entityTransform.Scale = scale;
		}
	}

	void GuiRenderer::DrawVector3Drag(const char* label, glm::vec3& vector, float panelWidth)
	{
		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, panelWidth);
		ImGui::Text("%s", label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		ImGui::AlignTextToFramePadding();
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::DragFloat("##X", &vector.x, 0.1f);
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &vector.y, 0.1f);
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &vector.z, 0.1f);
		ImGui::PopItemWidth();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void GuiRenderer::SetContext(Scene* context)
	{
		m_Context = context;
		m_SelectedEntity = Entity();
	}
}