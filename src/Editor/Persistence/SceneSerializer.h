#pragma once

#include "Editor/Scene/Scene.h"

namespace RenderingEngine
{
	class SceneSerializer
	{
	public:
		static void Serialize(const Ref<Scene>& scene, const std::string& filepath);

		static void Deserialize(const Ref<Scene>& scene, const std::string& filepath);
	};
}