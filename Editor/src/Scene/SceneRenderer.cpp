#include "SceneRenderer.h"

#include <glm/gtc/type_ptr.hpp>

namespace Editor
{
	SceneRenderer::SceneRenderer()
	{
		m_ShadowMapShader = Ptr<Shader>::Create(
				RESOURCES_PATH "Shaders/shadowMap.vert",
				RESOURCES_PATH "Shaders/shadowMap.frag");
	}

	void SceneRenderer::DrawDepthBuffer() const
	{
		const auto& meshRenderers = m_Context->m_Entities.view<MeshComponent, MaterialComponent, TransformComponent>();
		const auto& lights = m_Context->m_Entities.view<DirectLightComponent, TransformComponent>();

		if (lights.size_hint() != 1)
			return;

		const auto& lightEntity = lights.begin();
		const auto& [lightComponent, lightTransform] = lights.get<DirectLightComponent, TransformComponent>(*lightEntity);

		lightComponent.DepthBuffer->Bind();

		glClear(GL_DEPTH_BUFFER_BIT);

		const glm::mat4 view = glm::lookAt(lightTransform.Position, glm::vec3(0), glm::vec3(0, 1, 0));

		//TODO: batching
		for (const auto& meshEntity : meshRenderers)
		{
			const auto& [meshComponent, materialComponent, transformComponent] = meshRenderers.get<MeshComponent, MaterialComponent, TransformComponent>(meshEntity);

			materialComponent.Shared->BindMat4Uniform("u_lightViewProj", lightComponent.GetProjection() * view);

			Renderer::RenderMesh(meshComponent.SharedMesh, materialComponent.Shared, transformComponent.GetTrsMatrix());
		}

		lightComponent.DepthBuffer->Unbind();
	}

	void SceneRenderer::DrawTextureBuffer() const
	{
		const auto& renderCamera = m_Context->GetRenderCamera();
		const auto& cameraBuffer = renderCamera->GetFramebuffer();
		Entity cameraEntity = m_Context->GetActiveCameraEntity();

		cameraBuffer->Bind();
		Renderer::Clear(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));

		if (!cameraEntity)
		{
			cameraBuffer->Unbind();
			return;
		}

		const auto camera = cameraEntity.GetComponent<CameraComponent>();
        Renderer::UpdateCameraMatrix(renderCamera->GetProjMat() * glm::lookAt(camera.Position, camera.Position + camera.Orientation, Vector3::Up()));

		//TODO: batching
		cameraBuffer->ClearAttachment(INTEGER, -1);

		const auto& skyboxes = m_Context->m_Entities.view<SkyboxComponent>();
		for (const auto& skybox : skyboxes)
		{
			auto& skyboxComponent = skyboxes.get<SkyboxComponent>(skybox);
			skyboxComponent.Position = camera.Position;

			Renderer::RenderMeshEntity(skyboxComponent.Mesh, skyboxComponent.Material, skyboxComponent.GetProj(), (int)skybox);
		} 

		const auto& meshRenderers = m_Context->m_Entities.view<MeshComponent, MaterialComponent, TransformComponent>();
		for (const auto& meshEntity : meshRenderers)
		{
			const auto& modelMat = meshRenderers.get<MaterialComponent>(meshEntity).Shared;
			const auto& [meshComponent, transformComponent] = meshRenderers.get<MeshComponent, TransformComponent>(meshEntity);

			Renderer::RenderMeshEntity(meshComponent.SharedMesh, modelMat, transformComponent.GetTrsMatrix(), (int)meshEntity);
		}

		cameraBuffer->Unbind();
	}

	void SceneRenderer::SetContext(Ptr<Scene> context)
	{
		m_Context = std::move(context);
	}
}