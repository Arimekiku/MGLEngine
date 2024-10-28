#include "SceneRenderer.h"
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

		bool isOpened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, "%s", label);

		ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
		if (ImGui::Button("::", ImVec2(25, 20)))
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
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));

			T& component = entity.GetComponent<T>();
			func(component);

			ImGui::TreePop();
			ImGui::PopStyleVar();
		}

		if (removeRequested)
		{
			entity.RemoveComponent<T>();
		}
	}

	template<typename T>
	static void DrawAddComponent(const char* label, Entity entity)
	{
		if (entity.HasComponent<T>() == true)
		{
			return;
		}

		if (ImGui::MenuItem(label))
		{
			entity.AddComponent<T>();
			ImGui::CloseCurrentPopup();
		}
	}

	SceneRenderer::SceneRenderer()
	{
		m_ShadowMapShader = std::make_shared<Shader>(
				RESOURCES_PATH "Shaders/shadowMap.vert",
				RESOURCES_PATH "Shaders/shadowMap.frag");
	}

	void SceneRenderer::DrawDepthBuffer()
	{
		const auto& meshRenderers = m_Context->m_Entities.view<MeshComponent, TransformComponent>();

		m_DepthMap.Bind();

        Renderer::Clear(glm::vec4(0, 0, 0, 1));

		//TODO: batching
		//Draw call for shadow map
		for (const auto& meshEntity : meshRenderers)
		{
			const auto& [meshComponent, transformComponent] = meshRenderers.get<MeshComponent, TransformComponent>(meshEntity);

			Renderer::RenderMesh(meshComponent.SharedMesh, m_ShadowMapShader, transformComponent.GetTRSMatrix());
		}

		m_DepthMap.Unbind();
	}

	void SceneRenderer::DrawTextureBuffer()
	{
		Entity cameraEntity = m_Context->GetActiveCameraEntity();

		m_Viewport.Bind();

        Renderer::Clear(glm::vec4(0, 0, 0, 1));

		if (!cameraEntity)
		{
			m_Viewport.Unbind();
			return;
		}

		CameraComponent camera = cameraEntity.GetComponent<CameraComponent>();
        Renderer::UpdateCameraMatrix(m_ViewportCamera->GetProjMat() * glm::lookAt(camera.Position, camera.Position + camera.Orientation, Vector3::Up()));

		const auto& meshRenderers = m_Context->m_Entities.view<MeshComponent, MaterialComponent, TransformComponent>();

		//TODO: batching
		//Draw call for actual models
		for (const auto& meshEntity : meshRenderers)
		{
			Ref<Material> modelMat = meshRenderers.get<MaterialComponent>(meshEntity).SharedMat;

			modelMat->Bind();
			//TODO: think about it
			glBindTextureUnit(1, m_DepthMap.GetAttachment(0));

			const auto& [meshComponent, transformComponent] = meshRenderers.get<MeshComponent, TransformComponent>(meshEntity);
			Renderer::RenderMesh(meshComponent.SharedMesh, modelMat->GetShader(), transformComponent.GetTRSMatrix());
		}

		m_Viewport.Unbind();
	}

	void SceneRenderer::DrawViewport()
	{
		ImGui::Begin("Viewport");

        const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        const auto castSize = glm::i16vec2(viewportSize.x, viewportSize.y);

        if (castSize.x != 0 && castSize.y != 0)
        {
            if (glm::i16vec2(m_Viewport.GetWidth(), m_Viewport.GetHeight()) != castSize)
            {
				m_ViewportCamera->Resize(castSize.x, castSize.y);
				m_Viewport.Resize(castSize.x, castSize.y);
				m_DepthMap.Resize(castSize.x, castSize.y);
            }

            uint32_t m_Texture = m_Viewport.GetAttachment(0);
            ImGui::Image((void*)m_Texture, ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 0), ImVec2(1, -1));
        }

		if (m_SelectedEntity)
		{
			DrawGuizmos();
		}

        ImGui::End();
	}

	void SceneRenderer::DrawScenePanel()
	{
		ImGui::Begin("Scene");

		const auto& sceneEntities = m_Context->m_Entities.view<NameComponent>();

		for (const auto& entityID : sceneEntities)
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
				ImGui::CloseCurrentPopup();
			} 

			if (ImGui::BeginMenu("Primitives"))
            {
				if (ImGui::MenuItem("Create Plane"))
				{
					Entity entity = m_Context->Instantiate("Plane");
					entity.AddComponent<TransformComponent>();

					auto& material = MaterialImporter::GetMaterial("Default");
					entity.AddComponent<MaterialComponent>(material);
					
					auto& mesh = MeshImporter::CreatePlane();
					entity.AddComponent<MeshComponent>(mesh);

					m_SelectedEntity = entity;
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Create Cube"))
				{
					Entity entity = m_Context->Instantiate("Cube");
					entity.AddComponent<TransformComponent>();

					auto& material = MaterialImporter::GetMaterial("Default");
					entity.AddComponent<MaterialComponent>(material);
					
					auto& mesh = MeshImporter::CreateCube();
					entity.AddComponent<MeshComponent>(mesh);

					m_SelectedEntity = entity;
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Create Sphere"))
				{
					Entity entity = m_Context->Instantiate("Sphere");
					entity.AddComponent<TransformComponent>();

					auto& material = MaterialImporter::GetMaterial("Default");
					entity.AddComponent<MaterialComponent>(material);
					
					auto& mesh = MeshImporter::CreateSphere();
					entity.AddComponent<MeshComponent>(mesh);

					m_SelectedEntity = entity;
					ImGui::CloseCurrentPopup();
				}

                ImGui::EndMenu();
            }

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void SceneRenderer::DrawInspectorPanel()
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
				ImGui::AlignTextToFramePadding();
				DrawVector3Drag("Position", component.Position, 100.0f);

				ImGui::AlignTextToFramePadding();
				DrawVector3Drag("Rotation", component.Rotation, 100.0f);

				ImGui::AlignTextToFramePadding();
				DrawVector3Drag("Scale", component.Scale, 100.0f);
			});

			DrawComponent<MaterialComponent>("Material", m_SelectedEntity, [](MaterialComponent& component)
			{
				auto& material = component.SharedMat;
			});

			DrawComponent<MeshComponent>("Mesh", m_SelectedEntity, [](MeshComponent& component)
			{
				ImGui::AlignTextToFramePadding();
				ImGui::Text("%p\n", (void *) &component.SharedMesh);
			});

			DrawComponent<CameraComponent>("Camera", m_SelectedEntity, [](CameraComponent& component)
			{
				ImGui::AlignTextToFramePadding();
				DrawVector3Drag("Position", component.Position, 100.0f);

				ImGui::Checkbox("Enabled", &component.Enabled);
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
					if (m_SelectedEntity.HasComponent<CameraComponent>() == false)
					{
						DrawAddComponent<TransformComponent>("Transform", m_SelectedEntity);
					}

					if (m_SelectedEntity.HasComponent<TransformComponent>() == false)
					{
						DrawAddComponent<CameraComponent>("Camera", m_SelectedEntity);
					}
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	void SceneRenderer::DrawGuizmos() 
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

		glm::mat4 cameraView = glm::lookAt(camera.Position, camera.Position + camera.Orientation, Vector3::Up());
		glm::mat4 cameraProj = m_ViewportCamera->GetProjMat();

		TransformComponent& entityTransform = m_SelectedEntity.GetComponent<TransformComponent>();
		glm::mat4 entityTRS = entityTransform.GetTRSMatrix();

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProj), m_GuizmoOperation, ImGuizmo::LOCAL, glm::value_ptr(entityTRS));

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

	void SceneRenderer::OnEvent(Event& e)
	{
		m_ViewportCamera->OnEvent(e);
	}

	void SceneRenderer::SetContext(Ref<Scene> context)
	{
		m_Context = context;
		m_SelectedEntity = Entity();
	}

	void SceneRenderer::OnResizeEvent(WindowResizeEvent& e)
	{
		glm::vec2 size = glm::vec2(e.GetWidth(), e.GetHeight());

		m_ViewportCamera->Resize(size.x, size.y);
		m_Viewport.Resize(size.x, size.y);
		m_DepthMap.Resize(size.x, size.y);
	}
}