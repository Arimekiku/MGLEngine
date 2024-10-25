#pragma once

#include "RendererEngine.h"

#include <entt/entt.hpp>

namespace RenderingEngine 
{
	class Entity;

	class Scene
	{
	public: 
		Scene();
		~Scene() = default;

		void OnEveryUpdate(Time deltaTime);

		Entity Instantiate(const std::string& name = std::string());

	private:
		entt::registry m_Entities;

		Ref<Shader> m_ShadowMapShader;

		Framebuffer m_DepthMap = Framebuffer(1024, 1024, FramebufferType::DepthBuffer);
		Framebuffer m_Viewport = Framebuffer(800, 600, FramebufferType::Viewport);

		friend class Entity;
		friend class GuiRenderer;
	};
}