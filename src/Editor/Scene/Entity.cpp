#include "Entity.h"

namespace RenderingEngine 
{
	Entity::Entity(entt::entity handle, Scene* parent) 
	{
		m_EntityID = handle;
		m_Scene = parent;
	}
}