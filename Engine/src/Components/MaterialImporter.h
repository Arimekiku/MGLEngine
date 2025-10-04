#pragma once

#include "Core.h"
#include "Components/Material.h"

namespace Engine
{
	class MaterialImporter
	{
	public:
		static const Ref<Material>& GetMaterial(const std::string& name);
		static const Ref<Material>& AddMaterial(const std::string& name, const std::string& vertPath, const std::string& fragPath);
	};
}