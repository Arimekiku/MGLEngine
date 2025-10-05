#include "SceneRenderer.h"

#include <iostream>

#include "ComponentRegistry.h"
#include "Core/Math.h"

#include <glm/gtc/type_ptr.hpp>
#include <utility>

namespace Engine
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
			return;

		auto flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen
				   | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;

		bool isOpened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, "%s", label);

		ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
		if (ImGui::Button("::", ImVec2(25, 20)))
			ImGui::OpenPopup("parameters");

		bool removeRequested = false;
		if (ImGui::BeginPopup("parameters")) 
		{
			if (ImGui::MenuItem("Remove Component"))
				removeRequested = true;

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
			entity.RemoveComponent<T>();
	}

	template<typename T>
	static void DrawAddComponent(const char* label, Entity entity)
	{
		if (entity.HasComponent<T>() == true)
			return;

		if (ImGui::MenuItem(label))
		{
			entity.AddComponent<T>();
			ImGui::CloseCurrentPopup();
		}
	}

	SceneRenderer::SceneRenderer()
	{
		m_ShadowMapShader = Ptr<Shader>::Create(
				RESOURCES_PATH "Shaders/shadowMap.vert",
				RESOURCES_PATH "Shaders/shadowMap.frag");
	}

	void SceneRenderer::DrawDepthBuffer() const
	{
		const auto& meshRenderers = m_Context->m_Entities.view<MeshComponent, TransformComponent>();
		const auto& lights = m_Context->m_Entities.view<DirectLightComponent, TransformComponent>();

		for (const auto& lightEntity : lights)
		{
			const auto& [lightComponent, lightTransform] = lights.get<DirectLightComponent, TransformComponent>(lightEntity);

			lightComponent.DepthBuffer.Bind();
			glClear(GL_DEPTH_BUFFER_BIT);

			const glm::mat4 view = glm::lookAt(lightTransform.Position, glm::vec3(0), glm::vec3(0, 1, 0));
			m_ShadowMapShader->Bind();
			m_ShadowMapShader->BindUniformMat4("u_lightViewProj", lightComponent.GetProjection() * view);

			//TODO: batching
			//Draw call for shadow map
			for (const auto& meshEntity : meshRenderers)
			{
				const auto& [meshComponent, transformComponent] = meshRenderers.get<MeshComponent, TransformComponent>(meshEntity);

				Renderer::RenderMesh(meshComponent.SharedMesh, m_ShadowMapShader, transformComponent.GetTrsMatrix());
			}

			lightComponent.DepthBuffer.Unbind();
		}
	}

	void SceneRenderer::DrawTextureBuffer() const
	{
		const Framebuffer& cameraBuffer = m_ViewportCamera->GetFramebuffer();
		Entity cameraEntity = m_Context->GetActiveCameraEntity();

		cameraBuffer.Bind();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (!cameraEntity)
		{
			cameraBuffer.Unbind();
			return;
		}

		const auto camera = cameraEntity.GetComponent<CameraComponent>();
        Renderer::UpdateCameraMatrix(m_ViewportCamera->GetProjMat() * glm::lookAt(camera.Position, camera.Position + camera.Orientation, Vector3::Up()));

		//Draw call for skybox
		auto mesh = m_Context->GetSkyMesh();
		auto transform = m_Context->GetSkyTransform();
		transform->Position = camera.Position;
		auto mat = m_Context->GetSkyMaterial();
		const auto cubemap = m_Context->GetCubemap();
		mat->Bind();
		Renderer::RenderCubeMap(mesh->SharedMesh, mat->GetShader(), cubemap, transform->GetTrsMatrix());

		const auto& meshRenderers = m_Context->m_Entities.view<MeshComponent, MaterialComponent, TransformComponent>();
		const auto& lights = m_Context->m_Entities.view<DirectLightComponent, TransformComponent>();

		/*TODO: batching
		Draw call for actual models*/
		for (const auto& lightEntity : lights)
		{
			DirectLightComponent& lightComponent = lights.get<DirectLightComponent>(lightEntity);
			TransformComponent& lightTransform = lights.get<TransformComponent>(lightEntity);

			//TODO: batching
			//Draw call for shadow map
			for (const auto& meshEntity : meshRenderers)
			{
				const auto& modelMat = meshRenderers.get<MaterialComponent>(meshEntity).Shared;
				modelMat->Bind();
				modelMat->GetShader()->BindUniformFloat3("u_LightColor", lightComponent.Color);
				modelMat->GetShader()->BindUniformFloat3("u_LightPos", lightTransform.Position);

				const glm::mat4 view = glm::lookAt(lightTransform.Position, glm::vec3(0), glm::vec3(0, 1, 0));
				modelMat->GetShader()->BindUniformMat4("u_lightViewProj", lightComponent.GetProjection() * view);
				//TODO: think about it
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, lightComponent.DepthBuffer.GetColorAttachment());

				const auto& [meshComponent, transformComponent] = meshRenderers.get<MeshComponent, TransformComponent>(meshEntity);
				Renderer::RenderMesh(meshComponent.SharedMesh, modelMat->GetShader(), transformComponent.GetTrsMatrix());
				modelMat->Unbind();
			}
		}

		cameraBuffer.Unbind();
	}

	void SceneRenderer::DrawViewport()
	{
		if (ImGui::Begin("Viewport"))
		{
			const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			const auto castSize = glm::ivec2(viewportSize.x, viewportSize.y);

			if (castSize.x != 0 && castSize.y != 0)
			{
				if (m_ViewportCamera->GetSize() != castSize)
				{
					m_ViewportCamera->Resize(castSize.x, castSize.y);
					m_DepthMap.Resize(castSize.x, castSize.y);
				}

				const uint32_t texture = m_ViewportCamera->GetFramebuffer().GetColorAttachment();
				ImGui::Image((ImTextureID)texture, ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 0), ImVec2(1, -1));
			}

			if (m_SelectedEntity)
				DrawGuizmos();
		}

        ImGui::End();
	}

	void SceneRenderer::DrawScenePanel()
	{
		ImGui::Begin("Scene");

		const auto& sceneEntities = m_Context->m_Entities.view<NameComponent>();
		for (const auto& entityId : sceneEntities)
		{
			auto entity = Entity(entityId, m_Context.GetRaw());
			auto entityName = sceneEntities.get<NameComponent>(entity);

			ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow
									 | ImGuiTreeNodeFlags_SpanAvailWidth;

			const bool isOpened = ImGui::TreeNodeEx((void*)entityId, flags, "%s", entityName.Name.c_str());

			if (ImGui::IsItemClicked(0))
				m_SelectedEntity = Entity(entity, m_Context.GetRaw());

			bool deleteRequested = false;
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Destroy"))
					deleteRequested = true;

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
					m_SelectedEntity = Entity();

				m_Context->Destroy(entity);
			}
		}

		if (ImGui::BeginPopupContextWindow(nullptr, 1 | ImGuiPopupFlags_NoOpenOverItems))
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

				char buf[256] = {};
				strcpy_s(buf, nameComponent.Name.c_str());

				if (ImGui::InputText("Name", buf, sizeof(buf)))
					nameComponent.Name = buf;
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
				auto& material = component.Shared;
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

			DrawComponent<DirectLightComponent>("Direct Light", m_SelectedEntity, [](DirectLightComponent& component)
			{
				ImGui::ColorEdit3("Color", glm::value_ptr(component.Color));

				const uint32_t texture = component.DepthBuffer.GetColorAttachment();
				ImGui::Image((ImTextureID)texture, ImVec2(256, 256), ImVec2(0, 0), ImVec2(1, -1));
			});

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("addcomponent");

			if (ImGui::BeginPopup("addcomponent"))
			{
				bool entityHasCamera = m_SelectedEntity.HasComponent<CameraComponent>();
				bool entityHasTransform = m_SelectedEntity.HasComponent<TransformComponent>();

				if (entityHasCamera == false)
				{
					if (m_SelectedEntity.HasComponent<CameraComponent>() == false)
						DrawAddComponent<TransformComponent>("Transform", m_SelectedEntity);

					if (m_SelectedEntity.HasComponent<TransformComponent>() == false)
						DrawAddComponent<CameraComponent>("Camera", m_SelectedEntity);
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
			return;

		if (Input::Key::Pressed(Input::Key::F1))
			m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

		if (Input::Key::Pressed(Input::Key::F2))
			m_GuizmoOperation = ImGuizmo::OPERATION::ROTATE;

		if (Input::Key::Pressed(Input::Key::F3))
			m_GuizmoOperation = ImGuizmo::OPERATION::SCALE;

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		auto pos = ImGui::GetWindowPos();
		auto size = ImGui::GetWindowSize();
		ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);

		const auto camera = cameraEntity.GetComponent<CameraComponent>();
		glm::mat4 cameraView = glm::lookAt(camera.Position, camera.Position + camera.Orientation, Vector3::Up());
		glm::mat4 cameraProj = m_ViewportCamera->GetProjMat();

		auto& entityTransform = m_SelectedEntity.GetComponent<TransformComponent>();
		glm::mat4 entityTrs = entityTransform.GetTrsMatrix();

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProj), m_GuizmoOperation, ImGuizmo::LOCAL, glm::value_ptr(entityTrs));

		if (ImGuizmo::IsUsing()) 
		{
			glm::vec3 position, rotation, scale;

			if (Math::Decompose(entityTrs, position, rotation, scale) == false) 
				return;

			entityTransform.Position = position;

			const glm::vec3 deltaRot = rotation - glm::radians(entityTransform.Rotation);
			entityTransform.Rotation += glm::degrees(deltaRot);

			entityTransform.Scale = scale;
		}
	}

	void SceneRenderer::SetContext(Ptr<Scene> context)
	{
		m_Context = std::move(context);
		m_SelectedEntity = Entity();
	}
}