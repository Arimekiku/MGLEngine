#pragma once

#include "Core/Time.h"
#include "entt/entity/registry.hpp"

namespace Editor
{
	class System
	{
	public:
		System() = delete;
		explicit System(entt::registry& registry) : m_Registry(registry) {}
		virtual ~System() = default;

		virtual void OnEveryUpdate(Engine::Time deltaTime) {}
		virtual void OnGuiUpdate() const {}

	protected:
		entt::registry& m_Registry;
	};

	class LightingSystem final : public System
	{
	public:
		LightingSystem() = delete;
		explicit LightingSystem(entt::registry& registry) : System(registry) {}

		void OnEveryUpdate(Engine::Time deltaTime) override;
	};
}
