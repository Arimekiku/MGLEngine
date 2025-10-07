#include "SceneRenderingPanel.h"

#include <imgui.h>

namespace Editor
{
	void ScenePanel::DrawAsMenuItem()
	{
		ImGui::MenuItem(m_PanelName.c_str(), "", &m_IsActive);
	}

	void ScenePanel::Begin() const
	{
		ImGui::Begin(m_PanelName.c_str());
	}

	void ScenePanel::End() const
	{
		ImGui::End();
	}
}
