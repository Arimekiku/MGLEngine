#pragma once

#include "Entity.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

namespace RenderingEngine
{
	class GuiRenderer
	{
	public:
		GuiRenderer() = default;

		void DrawViewport();
		void DrawScenePanel();
		void DrawInspectorPanel();
		void DrawGuizmos();

		void SetContext(Scene* context);

	private:
		void DrawVector3Drag(const char* label, glm::vec3& vector, float panelWidth);

		Scene* m_Context = nullptr;
		Entity m_SelectedEntity = {};

		ImGuizmo::OPERATION m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	};
}