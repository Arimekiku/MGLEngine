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

		std::vector<Ptr<System>>& GetSystems() { return m_Systems; }

		Entity GetActiveCameraEntity();

	private:
		void OnKeyPressedEvent(const KeyPressedEvent& e);
		void UpdateCamera(CameraComponent& mainCamera, Time deltaTime);

		std::vector<Ptr<System>> m_Systems;

		entt::registry m_Entities;
		bool m_CameraEditorMode = true;

		friend class Entity;
		friend class SceneRenderer;
		friend class SceneSerializer;
	};
}
