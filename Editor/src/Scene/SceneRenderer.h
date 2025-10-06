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

		void DrawViewport();
		void DrawScenePanel();
		void DrawInspectorPanel();
		void DrawGuizmos();

		void SetContext(Ptr<Scene> context);

	private:
		Ptr<RenderCamera> m_ViewportCamera = Ptr<RenderCamera>::Create();
		Ptr<Scene> m_Context = nullptr;
		Ptr<Shader> m_ShadowMapShader = nullptr;

		Entity m_SelectedEntity = Entity();

		ImGuizmo::OPERATION m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	};
}