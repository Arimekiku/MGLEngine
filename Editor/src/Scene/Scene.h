#pragma once

#include "Core/Time.h"
#include "Events/Event.h"
#include "ComponentRegistry.h"

#include <entt/entt.hpp>

#include "Skybox.h"
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

		Ptr<Skybox>& GetSkybox() { return m_Skybox; }

		Entity GetActiveCameraEntity();

	private:
		void OnKeyPressedEvent(const KeyPressedEvent& e);
		void UpdateCamera(CameraComponent& mainCamera, Time deltaTime);

		Ptr<Skybox> m_Skybox = Ptr<Skybox>::Create();

		std::vector<Ptr<System>> m_Systems;

		entt::registry m_Entities;
		bool m_CameraEditorMode = true;

		friend class Entity;
		friend class SceneRenderer;
		friend class SceneSerializer;
	};
}
