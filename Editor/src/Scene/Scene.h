#pragma once

#include "ComponentRegistry.h"
#include "Core/Time.h"
#include "Events/Event.h"

#include <entt/entt.hpp>

#include "Systems/System.h"

namespace Editor
{
	class Entity;

	class Scene
	{
	public: 
		Scene() = delete;
		Scene(EventDispatcher& dispatcher);
		~Scene() = default;

		void OnEveryUpdate(Time deltaTime);
		void OnGuiUpdate() const;

		Entity Instantiate(const std::string& name);
		void Destroy(Entity entity);

		Entity GetSelection();
		void SetSelection(Entity entity);

		std::vector<Ptr<System>>& GetSystems() { return m_Systems; }

		Ptr<RenderCamera>& GetRenderCamera() { return m_RenderCamera; }
		const Ptr<RenderCamera>& GetRenderCamera() const { return m_RenderCamera; }

		Entity GetActiveCameraEntity();

	private:
		void OnKeyPressedEvent(const KeyPressedEvent& e);
		void UpdateCamera(CameraComponent& mainCamera, Time deltaTime);

		Ptr<RenderCamera> m_RenderCamera = Ptr<RenderCamera>::Create();
		std::vector<Ptr<System>> m_Systems;

		bool m_CameraEditorMode = true;

		entt::registry m_Entities;
		entt::entity m_Selection = entt::null;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneRenderer;
		friend class SceneSerializer;
	};
}
