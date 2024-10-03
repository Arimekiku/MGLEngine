#pragma once

#include "RendererEngine.h"

namespace RenderingEngine
{
	class GuiRenderer
	{
	public:
		static void DrawImGuiModel(const Ref<Model> selectedEntity);

	private:
		static void DrawVector3Drag(const char* label, glm::vec3& vector, float panelWidth);
	};
}