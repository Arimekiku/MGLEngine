#pragma once

#include "ComponentRegistry.h"

namespace Editor
{
	class Skybox
	{
	public:
		Skybox();

		Ptr<Mesh>& GetMesh() { return m_Mesh; }
		Ptr<TransformComponent>& GetTransform() { return m_Transform; }
		Ptr<Material>& GetMaterial() { return m_Mat; }

		void ShouldUseTexture(bool value) { m_UseTexture = value; }

	private:
		bool m_UseTexture = true;

		Ptr<Mesh> m_Mesh;
		Ptr<Material> m_Mat;
		Ptr<TransformComponent> m_Transform;
		Ptr<Cubemap> m_Cubemap;
	};
}
