#include "AreaLighting.h"
#include "mxpch.h"
#include "AreaLighting.h"

namespace RenderingEngine
{
	AreaLighting::AreaLighting(glm::vec3 position, const Ref<Model>& model)
		: Position(position), m_Model(model)
	{
		auto transform = m_Model->GetTransform();
		transform->Position = position;
	}

	void AreaLighting::OnEveryUpdate() const
	{
		Renderer::RenderModel(m_Model);
	}
}