#include "SceneRenderingPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.inl>

namespace Editor
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

	void SceneHierarchyPanel::DrawPanel()
	{
		if (m_IsActive == false)
			return;

		DrawHierarchy();
		DrawInspector();
	}

	void SceneHierarchyPanel::DrawHierarchy()
	{
		ImGui::Begin("Scene");

		const auto& sceneEntities = m_Context->m_Entities.view<NameComponent>();
		for (const auto& entityId : sceneEntities)
		{
			auto entity = Entity(entityId, m_Context.GetRaw());
			auto entityName = sceneEntities.get<NameComponent>(entity);

			ImGuiTreeNodeFlags flags = (m_Context->GetSelection() == entity) ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow
				| ImGuiTreeNodeFlags_SpanAvailWidth;

			const bool isOpened = ImGui::TreeNodeEx((void*)entityId, flags, "%s", entityName.Name.c_str());

			if (ImGui::IsItemClicked(0))
			{
				const auto selection = Entity(entity, m_Context.GetRaw());

				m_Context->SetSelection(selection);
			}

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
				if (m_Context->GetSelection() == entity)
				{
					constexpr auto selection = Entity();

					m_Context->SetSelection(selection);
				}

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

					m_Context->SetSelection(entity);
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

					m_Context->SetSelection(entity);
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

					m_Context->SetSelection(entity);
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawInspector()
	{
		ImGui::Begin("Inspector");

		Entity selectedEntity = m_Context->GetSelection();
		if (selectedEntity)
		{
			if (selectedEntity.HasComponent<NameComponent>())
			{
				NameComponent& nameComponent = selectedEntity.GetComponent<NameComponent>();

				char buf[256] = {};
				strcpy_s(buf, nameComponent.Name.c_str());

				if (ImGui::InputText("Name", buf, sizeof(buf)))
					nameComponent.Name = buf;
			}

			DrawComponent<TransformComponent>("Transform", selectedEntity, [](TransformComponent& component)
			{
				ImGui::AlignTextToFramePadding();
				DrawVector3Drag("Position", component.Position, 100.0f);

				ImGui::AlignTextToFramePadding();
				DrawVector3Drag("Rotation", component.Rotation, 100.0f);

				ImGui::AlignTextToFramePadding();
				DrawVector3Drag("Scale", component.Scale, 100.0f);
			});

			DrawComponent<MaterialComponent>("Material", selectedEntity, [](MaterialComponent& component)
			{
				auto& material = component.Shared;
			});

			DrawComponent<MeshComponent>("Mesh", selectedEntity, [](MeshComponent& component)
			{
				ImGui::AlignTextToFramePadding();
				ImGui::Text("%p\n", (void*)&component.SharedMesh);
			});

			DrawComponent<CameraComponent>("Camera", selectedEntity, [](CameraComponent& component)
			{
				ImGui::AlignTextToFramePadding();
				DrawVector3Drag("Position", component.Position, 100.0f);

				ImGui::Checkbox("Enabled", &component.Enabled);
			});

			DrawComponent<DirectLightComponent>("Direct Light", selectedEntity, [](DirectLightComponent& component)
			{
				ImGui::ColorEdit3("Color", glm::value_ptr(component.Color));

				const uint32_t texture = component.DepthBuffer->GetAttachment(DEPTH24_STENCIL8);
				ImGui::Image((ImTextureID)texture, ImVec2(256, 256), ImVec2(0, 0), ImVec2(1, -1));
			});

			DrawComponent<SkyboxComponent>("Skybox", selectedEntity, [](SkyboxComponent& component)
			{
				const char* modes[] = { "Plain", "Image" };

				if (ImGui::Combo("Mode", &component.Mode, modes, IM_ARRAYSIZE(modes)))
				{
					component.Material->BindIntUniform("u_Mode", component.Mode);
				}

				ImGui::Separator();

				if (ImGui::ColorEdit3("Color", &component.Color.x))
				{
					component.Material->BindVec3Uniform("u_Albedo", component.Color);
				}
			});

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("addcomponent");

			if (ImGui::BeginPopup("addcomponent"))
			{
				bool entityHasCamera = selectedEntity.HasComponent<CameraComponent>();
				bool entityHasTransform = selectedEntity.HasComponent<TransformComponent>();

				if (entityHasCamera == false)
				{
					if (selectedEntity.HasComponent<CameraComponent>() == false)
						DrawAddComponent<TransformComponent>("Transform", selectedEntity);

					if (selectedEntity.HasComponent<TransformComponent>() == false)
						DrawAddComponent<CameraComponent>("Camera", selectedEntity);
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}
}
