#pragma once

#include "RendererEngine.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/fwd.hpp>

namespace RenderingEngine 
{
	struct NameComponent
	{
		std::string Name;

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string& name) : Name(name) {}
	};

	struct TransformComponent
	{
		glm::vec3 Position = glm::vec3(0);
		glm::vec3 Rotation = glm::vec3(0);
		glm::vec3 Scale = glm::vec3(1);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) 
			: Position(position), Rotation(rotation), Scale(scale) { }

		glm::mat4 GetTRSMatrix() const
		{
			const glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
			const glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(Rotation)));
			const glm::mat4 scaling = glm::scale(glm::mat4(1.0f), Scale);

			return translation * rotation * scaling;
		}
	};

	struct MeshComponent
	{
		Ref<Mesh> SharedMesh = nullptr;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(const Ref<Mesh>& mesh) : SharedMesh(mesh) { }
	};

	struct MaterialComponent
	{
		Ref<Material> SharedMat;

		MaterialComponent() = default;
		MaterialComponent(const MaterialComponent&) = default;
		MaterialComponent(const Ref<Material>& material) : SharedMat(material) { }
	};

	struct DirectLightComponent
	{
		Framebuffer DepthBuffer = Framebuffer(1024, 1024, FramebufferType::DepthBuffer);
		glm::vec3 Color = glm::vec3(1, 1, 1);

		DirectLightComponent() = default;
		DirectLightComponent(const DirectLightComponent&) = default;

		glm::mat4 GetProjection() const
		{
			const glm::mat4 ortho = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 0.01f, 100.0f);
			const glm::mat4 model = glm::mat4(1.0f);

			return ortho * model;
		}
	};

	struct AreaLightComponent
	{
		glm::vec3 Color = glm::vec3(1, 1, 1);
		float Intensity = 1;

		AreaLightComponent() = default;
		AreaLightComponent(const AreaLightComponent&) = default;
	};

	struct CameraComponent
	{
		glm::vec3 Orientation = glm::vec3(0, 0, -1);
		glm::vec3 Position = glm::vec3(0);
		float MinRenderDist = 0.01;
		float MaxRenderDist = 100.0f;
		float FOV = 45.0f;
		bool Enabled = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const bool enabled) : Enabled(enabled) { }
	};
}