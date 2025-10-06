#include "System.h"

#include <unordered_set>

#include "Scene/ComponentRegistry.h"

namespace Editor
{
	void LightingSystem::OnEveryUpdate(Time deltaTime)
	{
		const auto& meshRenderers = m_Registry.view<MeshComponent, MaterialComponent, TransformComponent>();
		const auto& lights = m_Registry.view<DirectLightComponent, TransformComponent>();

		if (lights.size_hint() != 1)
		{
			LOG_ERROR("System can't update light! Entities number {0}", lights.size_hint());
			return;
		}

		const auto& lightEntity = lights.begin();
		const auto& lightComponent = lights.get<DirectLightComponent>(*lightEntity);
		const auto& lightTransform = lights.get<TransformComponent>(*lightEntity);

		std::unordered_set<Ptr<Material>, PtrHash, PtrEqual> materials;
		for (const auto& meshEntity : meshRenderers)
		{
			const auto& modelMat = meshRenderers.get<MaterialComponent>(meshEntity).Shared;

			materials.insert(modelMat);
		}

		const glm::mat4 view = glm::lookAt(lightTransform.Position, glm::vec3(0), glm::vec3(0, 1, 0));
		for (const auto& material : materials)
		{
			material->Bind();
			//TODO: think about it
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, lightComponent.DepthBuffer.GetAttachment(DEPTH24_STENCIL8));

			material->BindVec3Uniform("u_LightColor", lightComponent.Color);
			material->BindVec3Uniform("u_LightPos", lightTransform.Position);
			material->BindMat4Uniform("u_lightViewProj", lightComponent.GetProjection() * view);
			material->Unbind();
		}
	}
}
