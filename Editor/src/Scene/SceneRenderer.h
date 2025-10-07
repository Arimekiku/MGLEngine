#pragma once

#include "Entity.h"

namespace Editor
{
	class SceneRenderer
	{
	public:
		SceneRenderer();

		void DrawDepthBuffer() const;
		void DrawTextureBuffer() const;

		void SetContext(Ptr<Scene> context);

	private:
		Ptr<Scene> m_Context = nullptr;
		Ptr<Shader> m_ShadowMapShader = nullptr;
	};
}