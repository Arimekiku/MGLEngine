#include "SceneRenderingPanel.h"

namespace Editor
{
	void SceneHierarchyPanel::DrawPanel()
	{
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
	}
}
