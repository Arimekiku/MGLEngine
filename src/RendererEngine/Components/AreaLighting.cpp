#include "AreaLighting.h"
#include "mxpch.h"
#include "AreaLighting.h"

namespace RenderingEngine
{
	AreaLighting::AreaLighting(glm::vec3 position, const Ref<Model>& model)
		: Position(position), m_Model(model)
	{
		auto& modelPosition = m_Model->GetPosition();
		modelPosition = position;
	}

	void AreaLighting::OnEveryUpdate() const
	{
		Renderer::RenderModel(m_Model);
	}
}