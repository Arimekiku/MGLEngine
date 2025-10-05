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
		Scene() = delete;
		Scene(EventDispatcher& dispatcher);
		~Scene() = default;

		void OnEveryUpdate(Time deltaTime);

		Entity Instantiate(const std::string& name = std::string());
		void Destroy(Entity entity);

		void SetSkyMesh(const Ptr<Mesh>& mesh) { m_Mesh = Ptr<MeshComponent>::Create(mesh); }
		Ptr<MeshComponent> GetSkyMesh() { return m_Mesh; }
		Ptr<TransformComponent> GetSkyTransform() { return m_Transform; }
		Ptr<Material> GetSkyMaterial() { return m_Mat; }
		Ptr<Cubemap> GetCubemap() { return m_Cubemap; }

		Entity GetActiveCameraEntity();

	private:
		void OnKeyPressedEvent(const KeyPressedEvent& e);
		void UpdateCamera(CameraComponent& mainCamera, Time deltaTime);

		Ptr<MeshComponent> m_Mesh;
		Ptr<Material> m_Mat;
		Ptr<TransformComponent> m_Transform = Ptr<TransformComponent>::Create();
		Ptr<Cubemap> m_Cubemap;

		entt::registry m_Entities;
		bool m_CameraEditorMode = true;

		friend class Entity;
		friend class SceneRenderer;
		friend class SceneSerializer;
	};
}
