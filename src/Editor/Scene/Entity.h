#pragma once

#include "Scene.h"

#include <entt/entt.hpp>

namespace RenderingEngine 
{
	class Entity 
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* parent);
		
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			LOG_CLIENT_ASSERT(this->HasComponent<T>() == false, "Entity %d already has component!", (int32_t)m_EntityID);

			T& component = m_Scene->m_Entities.emplace<T>(m_EntityID, std::forward<Args>(args)...);

			return component;
		}

		template<typename T>
		void RemoveComponent()
		{
			LOG_CLIENT_ASSERT(this.HasComponent<T>(), "Entity %d does not have component!", (int32_t)m_EntityID);
		
			m_Scene->m_Entities.remove<T>(m_EntityID);
		}

		template<typename T>
		bool HasComponent()
		{
			bool value = m_Scene->m_Entities.any_of<T>(m_EntityID);

			return value;
		}

		template<typename T>
		T& GetComponent()
		{
			LOG_CLIENT_ASSERT(this->HasComponent<T>(), "Entity does not have component!");

			T& component = m_Scene->m_Entities.get<T>(m_EntityID);

			return component;
		}

		operator bool() const { return m_EntityID != entt::null; }
		operator entt::entity() const { return m_EntityID; }
		operator uint32_t() const { return (uint32_t)m_EntityID; }
		operator int() const { return (int)m_EntityID; }

		bool operator==(const Entity& other) const { return m_EntityID == other.m_EntityID && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity m_EntityID = entt::null;
		Scene* m_Scene = nullptr;
	};
}