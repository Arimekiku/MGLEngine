#pragma once

#include "Entity.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

namespace Editor
{
	class SceneRenderer
	{
	public:
		SceneRenderer();

		void DrawDepthBuffer() const;
		void DrawTextureBuffer() const;

		void DrawSelection();
		void DrawViewport();
		void DrawScenePanel();
		void DrawInspectorPanel();
		void DrawGuizmos();

		void SetContext(Ptr<Scene> context);

	private:
		Ptr<RenderCamera> m_ViewportCamera = Ptr<RenderCamera>::Create();
		Ptr<Scene> m_Context = nullptr;
		Ptr<Shader> m_ShadowMapShader = nullptr;

		Entity m_HoveredEntity = Entity();
		Entity m_SelectedEntity = Entity();

		glm::vec2 m_ViewportBounds[2];

		ImGuizmo::OPERATION m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	};
}