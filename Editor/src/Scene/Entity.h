#pragma once

#include "Scene.h"

#include <entt/entt.hpp>

namespace Engine
{
	class Entity 
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* parent);
		
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			LOG_ASSERT(this->HasComponent<T>() == false, "Entity %d already has component!", (int32_t)m_EntityId);

			T& component = m_Scene->m_Entities.emplace<T>(m_EntityId, std::forward<Args>(args)...);
			return component;
		}

		template<typename T>
		void RemoveComponent()
		{
			LOG_ASSERT(this->HasComponent<T>(), "Entity %d does not have component!", (int32_t)m_EntityId);
		
			m_Scene->m_Entities.remove<T>(m_EntityId);
		}

		template<typename T>
		bool HasComponent() const
		{
			const bool value = m_Scene->m_Entities.any_of<T>(m_EntityId);

			return value;
		}

		template<typename T>
		T& GetComponent()
		{
			LOG_ASSERT(this->HasComponent<T>(), "Entity does not have component!");

			T& component = m_Scene->m_Entities.get<T>(m_EntityId);
			return component;
		}

		operator bool() const { return m_EntityId != entt::null; }
		operator entt::entity() const { return m_EntityId; }
		operator uint32_t() const { return (uint32_t)m_EntityId; }
		operator int() const { return (int)m_EntityId; }

		bool operator==(const Entity& other) const { return m_EntityId == other.m_EntityId && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity m_EntityId = entt::null;
		Scene* m_Scene = nullptr;
	};
}