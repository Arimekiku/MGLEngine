#include "mxpch.h"
#include "GuiRenderer.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace RenderingEngine
{
	void GuiRenderer::DrawImGuiModel(const Ref<Model> selectedEntity)
	{
		char buf[256];
		memset(buf, 0, 256);
		strcpy(buf, selectedEntity->GetName());

		if (ImGui::InputText("Name", buf, sizeof(buf)))
		{
			selectedEntity->SetName(buf);
		}

		auto flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		if (ImGui::TreeNodeEx((void*)typeid(Transform).hash_code(), flags, "Transform"))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));

			ImGui::AlignTextToFramePadding();
			DrawVector3Drag("Position", selectedEntity->GetPosition(), 100.0f);

			ImGui::AlignTextToFramePadding();
			DrawVector3Drag("Rotation", selectedEntity->GetRotation(), 100.0f);

			ImGui::AlignTextToFramePadding();
			DrawVector3Drag("Scale", selectedEntity->GetScale(), 100.0f);

			ImGui::PopStyleVar();
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx((void*)typeid(Material).hash_code(), flags, "Matetial"))
		{
			auto& material = selectedEntity->GetMaterial();
			auto& properties = material->GetProperties();
			ImGui::Image((void*)properties.AlbedoID, ImVec2(64, 64));

			ImGui::ColorEdit3("Albedo", glm::value_ptr(properties.Albedo));
			ImGui::SliderFloat("Rougness", &properties.Roughness, 0, 1, "%.2f");
			ImGui::SliderFloat("Metallic", &properties.Metallic, 0, 1, "%.2f");
			ImGui::SliderFloat("AO", &properties.AO, 0, 1, "%.2f");

			ImGui::TreePop();
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
}