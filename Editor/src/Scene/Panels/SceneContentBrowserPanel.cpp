#include "SceneRenderingPanel.h"

namespace Editor
{
	SceneContentBrowserPanel::SceneContentBrowserPanel(const std::string& name) : ScenePanel(name)
	{
		auto layout = ImageLayout(1, 1, 4);
		layout.Format = RGBA8;
		layout.Target = TEXTURE_2D;

		auto testTexture = Image(layout);
		unsigned char whitePixel[4] = { 255, 255, 255, 255 };
		testTexture.WritePixels(whitePixel);

		m_TestTexture = Ptr<Texture>::Create(testTexture);
		testTexture.WritePixels(nullptr);
	}

	void SceneContentBrowserPanel::DrawPanel()
	{
		if (m_CurrentFolder != std::filesystem::path(m_AssetFolder))
		{
			if (ImGui::Button("<"))
			{
				m_CurrentFolder = m_CurrentFolder.parent_path();
			}
		}

		const float panelWidth = ImGui::GetContentRegionAvail().x;
		int totalColumns = (int)(panelWidth / (164.0f));
		totalColumns = std::max(totalColumns, 1);

		ImGui::Columns(totalColumns, nullptr, false);
		for (auto& iterator : std::filesystem::directory_iterator(m_CurrentFolder))
		{
			const auto& path = iterator.path();
			auto relative = std::filesystem::relative(path, m_AssetFolder);
			std::string filename = relative.filename().string();

			ImGui::ImageButton(path.string().c_str(), (ImTextureID)m_TestTexture->GetRendererId(), { 128, 128 }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				const std::string stringPath = path.string();
				const char* dragSourceTarget = stringPath.c_str();

				ImGui::SetDragDropPayload("ContentBrowser", dragSourceTarget, (stringPath.size() + 1) * sizeof(char));
				ImGui::Text(filename.c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				if (iterator.is_directory())
					m_CurrentFolder /= path.filename();
			}

			ImGui::Text("%s", filename.c_str());

			ImGui::NextColumn();
		}
	}
}
