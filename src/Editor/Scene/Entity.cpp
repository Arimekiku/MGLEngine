#include "Entity.h"

namespace RenderingEngine 
{
	Entity::Entity(const entt::entity handle, Scene* parent) 
	{
		m_EntityId = handle;
		m_Scene = parent;
	}
}