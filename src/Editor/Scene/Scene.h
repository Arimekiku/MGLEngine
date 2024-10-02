#pragma once

#include <RendererEngine.h>

namespace RenderingEngine 
{
	class Scene
	{
	public: 
		Scene();
		~Scene();

		void OnEveryUpdate();
		void OnGUIUpdate();

		const Ref<Model>& Instantiate(const Ref<Mesh>& mesh, const glm::vec3(position));

	private:
		std::vector<Ref<Model>> m_Instances;

		Ref<Material> m_DefaultMaterial;
		Ref<AreaLighting> m_Light;

		Ref<Model> m_SelectedEntity;
	};
}