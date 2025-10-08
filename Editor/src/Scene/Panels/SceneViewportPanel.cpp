#include "SceneRenderingPanel.h"

#include <glm/gtc/type_ptr.inl>

#include "Persistence/SceneSerializer.h"

namespace Editor
{
	void SceneViewportPanel::DrawPanel()
	{
		DrawViewport();

		const auto minBounds = ImGui::GetWindowContentRegionMin();
		const auto maxBounds = ImGui::GetWindowContentRegionMax();
		const auto viewportOffset = ImGui::GetWindowPos();

		m_ViewportBounds[0] = { minBounds.x + viewportOffset.x, minBounds.y + viewportOffset.y };
		m_ViewportBounds[1] = { maxBounds.x + viewportOffset.x, maxBounds.y + viewportOffset.y };

		DrawGuizmos();

		if (ImGui::IsWindowHovered())
		{
			const auto guizmosUsed = ImGuizmo::IsUsing() || ImGuizmo::IsOver();

			if (Input::Mouse::ButtonPressed(0) && guizmosUsed == false)
				m_Context->SetSelection(m_HoveredEntity);
		}

		DrawSelection();
	}

	void SceneViewportPanel::DrawViewport()
	{
		const auto& renderCamera = m_Context->GetRenderCamera();
		const auto viewportSize = ImGui::GetContentRegionAvail();
		const auto castSize = glm::ivec2(viewportSize.x, viewportSize.y);

		if (castSize.x > 0 && castSize.y > 0)
		{
			if (renderCamera->GetSize() != castSize)
				renderCamera->Resize(castSize.x, castSize.y);

			const uint32_t texture = renderCamera->GetFramebuffer()->GetAttachment(RGBA8);
			ImGui::Image((ImTextureID)texture, ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowser");
			if (payload != nullptr)
			{
				const auto path = (const char*)payload->Data;

				SceneSerializer::Deserialize(m_Context, path);
			}

			ImGui::EndDragDropTarget();
		}
	}

	void SceneViewportPanel::DrawGuizmos()
	{
		const auto& renderCamera = m_Context->GetRenderCamera();
		Entity cameraEntity = m_Context->GetActiveCameraEntity();

		auto selectedEntity = m_Context->GetSelection();
		if (!selectedEntity || !cameraEntity || selectedEntity.HasComponent<TransformComponent>() == false)
			return;

		if (Input::Key::Pressed(Input::Key::F1))
			m_GuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

		if (Input::Key::Pressed(Input::Key::F2))
			m_GuizmoOperation = ImGuizmo::OPERATION::ROTATE;

		if (Input::Key::Pressed(Input::Key::F3))
			m_GuizmoOperation = ImGuizmo::OPERATION::SCALE;

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

		const auto camera = cameraEntity.GetComponent<CameraComponent>();
		glm::mat4 cameraView = glm::lookAt(camera.Position, camera.Position + camera.Orientation, Vector3::Up());
		glm::mat4 cameraProj = renderCamera->GetProjMat();

		auto& entityTransform = selectedEntity.GetComponent<TransformComponent>();
		glm::mat4 entityTrs = entityTransform.GetTrsMatrix();

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProj), m_GuizmoOperation, ImGuizmo::LOCAL, glm::value_ptr(entityTrs));

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 position, rotation, scale;

			if (Math::Decompose(entityTrs, position, rotation, scale) == false)
				return;

			entityTransform.Position = position;

			const glm::vec3 deltaRot = rotation - glm::radians(entityTransform.Rotation);
			entityTransform.Rotation += glm::degrees(deltaRot);

			entityTransform.Scale = scale;
		}
	}

	void SceneViewportPanel::DrawSelection()
	{
		const auto& target = m_Context->GetRenderCamera()->GetFramebuffer();
		target->Bind();

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;

		const auto viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;

		const auto mousePos = glm::ivec2((int)mx, (int)my);
		if (mousePos.x < 0 || mousePos.y < 0 || mousePos.x >= (int)viewportSize.x || mousePos.y >= (int)viewportSize.y)
		{
			target->Unbind();
			return;
		}

		//TODO: improve
		int pixel = target->GetPixel(1, mousePos.x, mousePos.y);
		m_HoveredEntity = pixel == -1 ? Entity() : Entity((entt::entity)pixel, m_Context.GetRaw());

		target->Unbind();
	}
}
