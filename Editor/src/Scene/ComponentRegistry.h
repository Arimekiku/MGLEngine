#pragma once

#include "Engine.h"

#include <glm/fwd.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Editor 
{
	using namespace Engine;

	struct NameComponent
	{
		std::string Name;

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(std::string name) : Name(std::move(name)) {}
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

		glm::mat4 GetTrsMatrix() const
		{
			const glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
			const glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(Rotation)));
			const glm::mat4 scaling = glm::scale(glm::mat4(1.0f), Scale);

			return translation * rotation * scaling;
		}
	};

	struct MeshComponent
	{
		Ptr<Mesh> SharedMesh = nullptr;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(const Ptr<Mesh>& mesh) : SharedMesh(mesh) { }
	};

	struct MaterialComponent
	{
		Ptr<Material> Shared = nullptr;

		MaterialComponent() = default;
		MaterialComponent(const MaterialComponent&) = default;
		MaterialComponent(const Ptr<Material>& material) : Shared(material) { }
	};

	struct DirectLightComponent
	{
		Ptr<Framebuffer> DepthBuffer = nullptr;
		glm::vec3 Color = glm::vec3(1, 1, 1);

		DirectLightComponent()
		{
			auto depthLayout = ImageLayout(4096, 4096, 1);
			depthLayout.Target = TEXTURE_2D;
			depthLayout.Format = DEPTH24_STENCIL8;
			depthLayout.Name = "LightDepth";
			const auto depthAttachment = Ptr<Texture>::Create(depthLayout);

			FramebufferLayout attachments = { depthAttachment };
			attachments.Width = 4096;
			attachments.Height = 4096;

			DepthBuffer = Ptr<Framebuffer>::Create(attachments);
		}

		glm::mat4 GetProjection() const
		{
			const auto ortho = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 0.01f, 100.0f);
			const auto model = glm::mat4(1.0f);

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
		float MinRenderDist = 0.01f;
		float MaxRenderDist = 100.0f;
		float Fov = 45.0f;
		bool Enabled = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const bool enabled) : Enabled(enabled) { }
	};

	struct SkyboxComponent
	{
		Ptr<Mesh> Mesh = nullptr;
		Ptr<Material> Material = nullptr;

		glm::vec3 Color = glm::vec3(1);
		glm::vec3 Position = glm::vec3(0);
		int Mode = 1;

		glm::mat4 GetProj() const
		{
			const glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
			const glm::mat4 scaling = glm::scale(glm::mat4(1.0f), glm::vec3(100));

			return translation * scaling;
		}
	};
}