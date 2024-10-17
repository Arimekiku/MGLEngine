#pragma once

#include <glm/glm.hpp>

namespace RenderingEngine
{
	class DirectionalLight
	{
	public:
		DirectionalLight();

		const glm::mat4& GetProjViewMat() const { return m_Parameters.LightMat; }

	private:
		struct DLParameters
		{
			glm::vec3 Position = glm::vec3(1.0f, 5.0f, -5.0f);

			glm::mat4 LightMat;
		};

		DLParameters m_Parameters;
	};
}