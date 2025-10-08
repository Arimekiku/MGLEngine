#pragma once

#include <filesystem>
#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

#include "Scene/Entity.h"
#include "Scene/Scene.h"

namespace Editor
{
	class ScenePanel
	{
	public:
		ScenePanel(std::string name) : m_PanelName(std::move(name)) { }
		virtual ~ScenePanel() = default;

		virtual void DrawPanel() { }

		void DrawAsMenuItem();
		void Begin() const;
		void End() const;

		void SetContext(const Ptr<Scene>& context) { m_Context = context; }

		bool IsActive() const { return m_IsActive; }

	protected:
		Ptr<Scene> m_Context;

		std::string m_PanelName;

	private:
		bool m_IsActive = true;
	};

	class SceneViewportPanel final : public ScenePanel
	{
	public:
		SceneViewportPanel(const std::string& name) : ScenePanel(name) { }

		void DrawPanel() override;

	private:
		void DrawViewport();
		void DrawGuizmos();
		void DrawSelection();

		Entity m_HoveredEntity = Entity();

		glm::vec2 m_ViewportBounds[2] = { };

		ImGuizmo::OPERATION m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	};

	class SceneHierarchyPanel final : public ScenePanel
	{
	public:
		SceneHierarchyPanel(const std::string& name) : ScenePanel(name) { }

		void DrawPanel() override;
	};

	class SceneInspectorPanel final : public ScenePanel
	{
	public:
		SceneInspectorPanel(const std::string& name) : ScenePanel(name) {}

		void DrawPanel() override;
	};

	class SceneContentBrowserPanel final : public ScenePanel
	{
	public:
		SceneContentBrowserPanel(const std::string& name);

		void DrawPanel() override;

	private:
		std::filesystem::path m_AssetFolder = RESOURCES_PATH;
		std::filesystem::path m_CurrentFolder = RESOURCES_PATH;

		Ptr<Texture> m_TestTexture = nullptr;
	};
}
