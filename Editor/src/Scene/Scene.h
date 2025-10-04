#pragma once

#include "Core/Time.h"
#include "Events/Event.h"
#include "ComponentRegistry.h"

#include <entt/entt.hpp>

namespace Engine
{
	class Entity;

	class Scene
	{
	public: 
		Scene();
		~Scene() = default;

		void OnEveryUpdate(Time deltaTime);

		Entity Instantiate(const std::string& name = std::string());
		void Destroy(Entity entity);

		Entity GetActiveCameraEntity();

	private:
		void OnKeyPressedEvent(const KeyPressedEvent& e);
		void UpdateCamera(CameraComponent& mainCamera, Time deltaTime);

		entt::registry m_Entities;
		bool m_CameraEditorMode = true;

		friend class Entity;
		friend class SceneRenderer;
		friend class SceneSerializer;
	};
}
