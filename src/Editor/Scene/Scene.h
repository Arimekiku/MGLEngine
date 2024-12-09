#pragma once

#include "ComponentRegistry.h"

#include <entt/entt.hpp>

namespace RenderingEngine 
{
	class Entity;

	class Scene
	{
	public: 
		Scene() = default;
		~Scene() = default;

		void OnEveryUpdate(Time deltaTime);
		void OnEvent(Event& e);

		Entity Instantiate(const std::string& name = std::string());
		void Destroy(Entity entity);

		Entity GetActiveCameraEntity();

	private:
		bool OnKeyPressedEvent(const KeyPressedEvent& e);
		void UpdateCamera(CameraComponent& mainCamera, Time deltaTime);

		entt::registry m_Entities;
		bool m_CameraEditorMode = true;

		friend class Entity;
		friend class SceneRenderer;
		friend class SceneSerializer;
	};
}