#pragma once

#include "Entity.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

namespace RenderingEngine
{
	class SceneRenderer
	{
	public:
		SceneRenderer();

		void DrawDepthBuffer();
		void DrawTextureBuffer();

		void DrawViewport();
		void DrawScenePanel();
		void DrawInspectorPanel();
		void DrawGuizmos();

		void OnEvent(Event& e);

		void SetContext(Ref<Scene> context);

	private:
		void OnResizeEvent(WindowResizeEvent& e);

		Framebuffer m_DepthMap = Framebuffer(1024, 1024, FramebufferType::DepthBuffer);
		Framebuffer m_Viewport = Framebuffer(800, 600, FramebufferType::Viewport);

		Ref<RenderCamera> m_ViewportCamera = std::make_shared<RenderCamera>();
		Ref<Scene> m_Context = nullptr;
		Ref<Shader> m_ShadowMapShader;

		Entity m_SelectedEntity = {};

		ImGuizmo::OPERATION m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	};
}