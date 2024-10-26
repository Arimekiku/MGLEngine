#include "GuiRenderer.h"
#include "ComponentRegistry.h"
#include "RendererEngine/Core/Math.h"

namespace RenderingEngine
{
	static void DrawVector3Drag(const char* label, glm::vec3& vector, float panelWidth)
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

	template<typename T, typename Func>
	static void DrawComponent(const char* label, Entity entity, Func func)
	{
		if (entity.HasComponent<T>() == false)
		{
			return;
		}

		auto flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen
				   | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;

		bool isOpened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, label);

		ImGui::SameLine();
		if (ImGui::Button("::"))
		{
			ImGui::OpenPopup("parameters");
		}

		bool removeRequested = false;
		if (ImGui::BeginPopup("parameters")) 
		{
			if (ImGui::MenuItem("Remove Component"))
			{
				removeRequested = true;
			}

			ImGui::EndPopup();
		}
				
		if (isOpened)
		{
			T& component = entity.GetComponent<T>();
			func(component);

			ImGui::TreePop();
		}

		if (removeRequested)
		{
			entity.RemoveComponent<T>();
		}
	}

	void GuiRenderer::DrawViewport() 
	{
		ImGui::Begin("Viewport");

        const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        const auto castSize = glm::i16vec2(viewportSize.x, viewportSize.y);

        if (castSize.x != 0 && castSize.y != 0)
        {
            if (glm::i16vec2(m_Context->m_Viewport.GetWidth(), m_Context->m_Viewport.GetHeight()) != castSize)
            {
				Entity cameraEntity = m_Context->GetActiveCameraEntity();

				if (cameraEntity)
				{
					CameraComponent camera = cameraEntity.GetComponent<CameraComponent>();
					camera.MainCamera->Resize(castSize.x, castSize.y);
					m_Context->m_Viewport.Resize(castSize.x, castSize.y);
					m_Context->m_DepthMap.Resize(castSize.x, castSize.y);
				}
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

		for (auto& entityID : sceneEntities)
		{
			Entity entity = Entity(entityID, m_Context.get());
			NameComponent entityName = sceneEntities.get<NameComponent>(entity);

			ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow
									 | ImGuiTreeNodeFlags_SpanAvailWidth;

			bool isOpened = ImGui::TreeNodeEx((void*)entityID, flags, "%s", entityName.Name.c_str());

			if (ImGui::IsItemClicked(0))
			{
				m_SelectedEntity = Entity(entity, m_Context.get());
			}

			bool deleteRequested = false;
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Destroy"))
				{
					deleteRequested = true;
				}

				ImGui::EndPopup();
			}

			if (isOpened)
			{
				//TODO: childens
				
				ImGui::TreePop();
			}

			if (deleteRequested)
			{
				if (m_SelectedEntity == entity)
				{
					m_SelectedEntity = Entity();
				}

				m_Context->Destroy(entity);
			}
		}

		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Blank"))
			{
				m_Context->Instantiate("Blank");
			}

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void GuiRenderer::DrawInspectorPanel()
	{
		ImGui::Begin("Inspector");

		if (m_SelectedEntity)
		{
			if (m_SelectedEntity.HasComponent<NameComponent>())
			{
				NameComponent& nameComponent = m_SelectedEntity.GetComponent<NameComponent>();

				char buf[256];
				memset(buf, 0, 256);
				strcpy(buf, nameComponent.Name.c_str());

				if (ImGui::InputText("Name", buf, sizeof(buf)))
				{
					nameComponent.Name = buf;
				}
			}

			DrawComponent<TransformComponent>("Transform", m_SelectedEntity, [](TransformComponent& component)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));

				ImGui::AlignTextToFramePadding();
				DrawVector3Drag("Position", component.Position, 100.0f);

				ImGui::AlignTextToFramePadding();
				DrawVector3Drag("Rotation", component.Rotation, 100.0f);

				ImGui::AlignTextToFramePadding();
				DrawVector3Drag("Scale", component.Scale, 100.0f);

				ImGui::PopStyleVar();
			});

			DrawComponent<MaterialComponent>("Material", m_SelectedEntity, [](MaterialComponent& component)
			{
				auto& material = component.SharedMat;
				auto& properties = material->GetProperties();

				ImGui::Image((void*)properties.AlbedoID, ImVec2(64, 64));

				ImGui::ColorEdit3("Albedo", glm::value_ptr(properties.Albedo));
				ImGui::SliderFloat("Rougness", &properties.Roughness, 0, 1, "%.2f");
				ImGui::SliderFloat("Metallic", &properties.Metallic, 0, 1, "%.2f");
				ImGui::SliderFloat("AO", &properties.AO, 0, 1, "%.2f");
			});

			DrawComponent<MeshComponent>("Mesh", m_SelectedEntity, [](MeshComponent& component)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));

				ImGui::AlignTextToFramePadding();
				ImGui::Text("%p\n", (void *) &component.SharedMesh);

				ImGui::PopStyleVar();
			});

			DrawComponent<CameraComponent>("Camera", m_SelectedEntity, [](CameraComponent& component)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));

				ImGui::AlignTextToFramePadding();
				DrawVector3Drag("Position", component.MainCamera->Position, 100.0f);

				ImGui::Checkbox("Enabled", &component.Enabled);

				ImGui::PopStyleVar();
			});

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("addcomponent");
			}

			if (ImGui::BeginPopup("addcomponent"))
			{
				bool entityHasCamera = m_SelectedEntity.HasComponent<CameraComponent>();
				bool entityHasTransform = m_SelectedEntity.HasComponent<TransformComponent>();

				if (entityHasCamera == false)
				{
					if (m_SelectedEntity.HasComponent<TransformComponent>() == false)
					{
						if (ImGui::MenuItem("Transform"))
						{
							m_SelectedEntity.AddComponent<TransformComponent>();
							ImGui::CloseCurrentPopup();
						}
					}
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	void GuiRenderer::DrawGuizmos() 
	{
		Entity cameraEntity = m_Context->GetActiveCameraEntity();

		if (!m_SelectedEntity || !cameraEntity || m_SelectedEntity.HasComponent<TransformComponent>() == false)
		{
			return;
		}

		if (Input::KeyPressed(GLFW_KEY_F1))
		{
			m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		}

		if (Input::KeyPressed(GLFW_KEY_F2))
		{
			m_GuizmoOperation = ImGuizmo::OPERATION::ROTATE;
		}

		if (Input::KeyPressed(GLFW_KEY_F3))
		{
			m_GuizmoOperation = ImGuizmo::OPERATION::SCALE;
		}

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		float width = ImGui::GetWindowWidth();
		float height = ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, width, height);

		CameraComponent camera = cameraEntity.GetComponent<CameraComponent>();
		glm::mat4 cameraView = camera.MainCamera->GetViewMat();
		glm::mat4 cameraProj = camera.MainCamera->GetProjMat();

		TransformComponent& entityTransform = m_SelectedEntity.GetComponent<TransformComponent>();
		glm::mat4 entityTRS = entityTransform.GetTRSMatrix();

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProj), 
		(ImGuizmo::OPERATION)m_GuizmoOperation, ImGuizmo::LOCAL, glm::value_ptr(entityTRS));

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

	void GuiRenderer::SetContext(Ref<Scene> context)
	{
		m_Context = context;
		m_SelectedEntity = Entity();
	}
}