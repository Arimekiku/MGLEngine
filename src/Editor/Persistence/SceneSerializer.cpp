#include "SceneSerializer.h"

#include "Editor/Scene/Entity.h"
#include "Editor/Scene/ComponentRegistry.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace RenderingEngine {
	static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << YAML::EndSeq;
		return out;
	}

	static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
		return out;
	}

	static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : m_Scene(scene)
	{

	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity;

		if (entity.HasComponent<NameComponent>())
		{
			out << YAML::Key << "NameComponent";
			out << YAML::BeginMap;

			auto& name = entity.GetComponent<NameComponent>().Name;
			out << YAML::Key << "Name" << YAML::Value << name;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& transform = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << transform.Position;
			out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();

			out << YAML::Key << "Position" << YAML::Value << cameraComponent.Position;
			out << YAML::Key << "Orientation" << YAML::Value << cameraComponent.Orientation;
			out << YAML::Key << "FOV" << YAML::Value << cameraComponent.FOV;
			out << YAML::Key << "MinRenderDist" << YAML::Value << cameraComponent.MinRenderDist;
			out << YAML::Key << "MaxRenderDist" << YAML::Value << cameraComponent.MaxRenderDist;
			out << YAML::Key << "Enabled" << YAML::Value << cameraComponent.Enabled;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<MeshComponent>())
		{
			out << YAML::Key << "MeshComponent";
			out << YAML::BeginMap;

			auto& meshComponent = entity.GetComponent<MeshComponent>();

			out << YAML::Key << "Path" << YAML::Value << meshComponent.SharedMesh->GetPath();

			out << YAML::EndMap;
		}

		if (entity.HasComponent<MaterialComponent>())
		{
			out << YAML::Key << "MaterialComponent";
			out << YAML::BeginMap;

			auto& materialComponent = entity.GetComponent<MaterialComponent>();
			auto& material = materialComponent.SharedMat;

			out << YAML::Key << "Name" << YAML::Value << material->GetName();
			out << YAML::Key << "VertPath" << YAML::Value << material->GetVertPath();
			out << YAML::Key << "FragPath" << YAML::Value << material->GetFragPath();

			out << YAML::EndMap;
		}

		if (entity.HasComponent<DirectLightComponent>())
		{
			out << YAML::Key << "DirectLightComponent";
			out << YAML::BeginMap;

			auto& directLightComponent = entity.GetComponent<DirectLightComponent>();

			out << YAML::Key << "Color" << YAML::Value << directLightComponent.Color;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<AreaLightComponent>())
		{
			out << YAML::Key << "AreaLightComponent";
			out << YAML::BeginMap;

			auto& areaLightComponent = entity.GetComponent<AreaLightComponent>();

			out << YAML::Key << "Color" << YAML::Value << areaLightComponent.Color;
			out << YAML::Key << "Intensity" << YAML::Value << areaLightComponent.Intensity;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "SampleScene";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		for (auto entityID : m_Scene->m_Entities.view<entt::entity>())
		{
			Entity entity = Entity(entityID, m_Scene.get());

			if (!entity)
			{
				return;
			}

			SerializeEntity(out, entity);
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			LOG_EDITOR_ERROR("Failed to load scene {0}\n{1}", filepath, e.what());
			return false;
		}

		if (!data["Scene"])
		{
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		LOG_EDITOR_TRACE("Deserializing scene {0}", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				std::string name;
				auto tagComponent = entity["NameComponent"];
				if (tagComponent)
				{
					name = tagComponent["Name"].as<std::string>();
				}

				LOG_EDITOR_TRACE("Deserialized entity {1}", name);

				Entity deserializedEntity = m_Scene->Instantiate(name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& transform = deserializedEntity.AddComponent<TransformComponent>();

					transform.Position = transformComponent["Position"].as<glm::vec3>();
					transform.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					transform.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& camera = deserializedEntity.AddComponent<CameraComponent>();

					camera.Position = cameraComponent["Position"].as<glm::vec3>();
					camera.Orientation = cameraComponent["Orientation"].as<glm::vec3>();
					camera.FOV = cameraComponent["FOV"].as<float>();
					camera.MinRenderDist = cameraComponent["MinRenderDist"].as<float>();
					camera.MaxRenderDist = cameraComponent["MaxRenderDist"].as<float>();
					camera.Enabled = cameraComponent["Enabled"].as<bool>();
				}

				auto meshComponent = entity["MeshComponent"];
				if (meshComponent)
				{
					auto& mesh = deserializedEntity.AddComponent<MeshComponent>();

					mesh.SharedMesh = MeshImporter::CreateMesh(meshComponent["Path"].as<std::string>().c_str());
				}

				auto directLightComponent = entity["DirectLightComponent"];
				if (directLightComponent)
				{
					auto& light = deserializedEntity.AddComponent<DirectLightComponent>();

					light.Color = directLightComponent["Color"].as<glm::vec3>();
				}

				auto areaLightComponent = entity["AreaLightComponent"];
				if (areaLightComponent)
				{
					auto& light = deserializedEntity.AddComponent<AreaLightComponent>();

					light.Color = areaLightComponent["Color"].as<glm::vec3>();
					light.Intensity = areaLightComponent["Intensity"].as<float>();
				}

				auto materialComponent = entity["MaterialComponent"];
				if (materialComponent)
				{
					std::string vertPath = materialComponent["VertPath"].as<std::string>();
					std::string fragPath = materialComponent["FragPath"].as<std::string>();
					std::string name = materialComponent["Name"].as<std::string>();

					Ref<Material> sharedMat = MaterialImporter::AddMaterial(name, vertPath, fragPath);
					auto& material = deserializedEntity.AddComponent<MaterialComponent>(sharedMat);
				}
			}
		}

		return true;
	}
}