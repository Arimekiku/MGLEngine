#pragma once

#include <RendererEngine.h>

namespace RenderingEngine 
{
	class Scene
	{
	public: 
		Scene();
		~Scene();

		void OnEveryUpdate(Time deltaTime);
		void OnGUIUpdate();

		Camera& GetSceneCamera() { return m_Camera; }

		const Ref<Model>& Instantiate(const Ref<Mesh>& mesh, const glm::vec3 position);

	private:
		void DrawScenePanel();
		void DrawInspectorPanel();

		std::vector<Ref<Model>> m_Instances;
		Ref<Material> m_DefaultMaterial;

		AreaLighting m_Light;
		Camera m_Camera;

		Ref<Model> m_SelectedEntity;
	};
}