#pragma once

#include "Scene/Scene.h"

namespace Engine
{
	class SceneSerializer
	{
	public:
		static void Serialize(const Ptr<Scene>& scene, const std::string& filepath);

		static void Deserialize(const Ptr<Scene>& scene, const std::string& filepath);
	};
}