#pragma once

#include <RendererEngine.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

namespace RenderingEngine 
{
	class Scene
	{
	public: 
		Scene();
		~Scene() = default;

		void OnEveryUpdate(Time deltaTime);
		void OnGUIUpdate();

		Camera& GetSceneCamera() { return m_Camera; }

		const Ref<Model>& Instantiate(const Ref<Mesh>& mesh, const glm::vec3 position);

	private:
		void DrawScenePanel();
		void DrawInspectorPanel();
		void DrawViewport();
		void DrawGuizmos();

		std::vector<Ref<Model>> m_Instances;
		Ref<Material> m_DefaultMaterial;
		Ref<Shader> m_ShadowMapShader;

		Framebuffer m_DepthMap = Framebuffer(1024, 1024, FramebufferType::DepthBuffer);
		Framebuffer m_Viewport = Framebuffer(800, 600, FramebufferType::Viewport);
		AreaLighting m_Light;
		DirectionalLight m_DirLight = DirectionalLight();
		Camera m_Camera;

		ImGuizmo::OPERATION m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

		Ref<Model> m_SelectedEntity;
	};
}