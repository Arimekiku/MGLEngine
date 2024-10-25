#include "Scene.h"
#include "GuiRenderer.h"
#include "RendererEngine/Core/Math.h"
#include "ComponentRegistry.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/trigonometric.hpp>
#include <entt/entt.hpp>

namespace RenderingEngine
{
	Scene::Scene()
	{
		m_ShadowMapShader = std::make_shared<Shader>(
			RESOURCES_PATH "Shaders/shadowMap.vert",
            RESOURCES_PATH "Shaders/shadowMap.frag");
	}

	Entity Scene::Instantiate(const std::string& name)
	{
		Entity entity = { m_Entities.create(), this };

		entity.AddComponent<NameComponent>(name.empty() ? "Entity" : name);
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	void Scene::OnEveryUpdate(Time deltaTime)
	{
		auto& meshRenderers = m_Entities.view<MeshComponent, TransformComponent>();

		glCullFace(GL_FRONT);
		m_DepthMap.Bind();

		//Draw call for shadow map
        Renderer::Clear(glm::vec4(0, 0, 0, 1));

		for (auto& meshEntity : meshRenderers)
		{
			auto& [meshComponent, transformComponent] = meshRenderers.get<MeshComponent, TransformComponent>(meshEntity);

			Renderer::RenderMesh(meshComponent.SharedMesh, m_ShadowMapShader, transformComponent.GetTRSMatrix());
		}

		Framebuffer::Unbind();
		glCullFace(GL_BACK);

		m_Viewport.Bind();

		//Draw call for actual models
        Renderer::Clear(glm::vec4(0, 0, 0, 1));

		for (auto& meshEntity : meshRenderers)
		{
			Ref<Material> modelMat;

			if (m_Entities.any_of<MaterialComponent>(meshEntity))
				modelMat = m_Entities.get<MaterialComponent>(meshEntity).SharedMat;

			modelMat->GetShader()->Bind();
			glBindTextureUnit(0, modelMat->GetProperties().AlbedoID);
			glBindTextureUnit(1, m_DepthMap.GetAttachment(0));

			auto& [meshComponent, transformComponent] = meshRenderers.get<MeshComponent, TransformComponent>(meshEntity);
			Renderer::RenderMesh(meshComponent.SharedMesh, modelMat->GetShader(), transformComponent.GetTRSMatrix());
		}

		Framebuffer::Unbind();
	}
}