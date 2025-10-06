#include "Scene.h"

#include <glm/trigonometric.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Entity.h"

namespace Editor
{
	void SetOrientation(CameraComponent& mainCamera, const float rotX, const float rotY)
	{
		glm::vec3& cameraOrientation = mainCamera.Orientation;

		// Calculates upcoming vertical change in the Orientation
		const glm::vec3 newOrientation = glm::rotate(cameraOrientation, glm::radians(-rotX),
			glm::normalize(glm::cross(cameraOrientation, Vector3::Up())));

		// Decides whether or not the next vertical Orientation is legal or not
		if (std::abs(glm::angle(newOrientation, Vector3::Up()) - glm::radians(90.0f)) <= glm::radians(85.0f))
			cameraOrientation = newOrientation;

		// Rotates the Orientation left and right
		cameraOrientation = glm::rotate(cameraOrientation, glm::radians(-rotY), Vector3::Up());
	}

	Scene::Scene(EventDispatcher& dispatcher)
	{
		const auto lightSystem = Ptr<LightingSystem>::Create(m_Entities);
		m_Systems.emplace_back(lightSystem);

		dispatcher.Subscribe<KeyPressedEvent>(BIND_FUNC(OnKeyPressedEvent));
	}

	Entity Scene::Instantiate(const std::string& name)
	{
		auto entity = Entity(m_Entities.create(), this);
		entity.AddComponent<NameComponent>(name.empty() ? "Entity" : name);

		return entity;
	}

	void Scene::Destroy(Entity entity)
	{
		m_Entities.destroy(entity);
	}

	Entity Scene::GetActiveCameraEntity()
	{
		const auto view = m_Entities.view<CameraComponent>();
		for (auto& cameraEntity : view)
		{
			const auto& camera = view.get<CameraComponent>(cameraEntity);
			if (camera.Enabled == false)
				continue;

			return Entity(cameraEntity, this);
		}

		return Entity();
	}

	void Scene::OnEveryUpdate(const Time deltaTime)
	{
		for (const auto& system : m_Systems)
			system->OnEveryUpdate(deltaTime);

		Entity mainCameraEntity = GetActiveCameraEntity();
		if (!mainCameraEntity)
			return;

		UpdateCamera(mainCameraEntity.GetComponent<CameraComponent>(), deltaTime);
	}

	void Scene::OnGuiUpdate() const
	{
		for (auto& system : m_Systems)
			system->OnGuiUpdate();
	}

	void Scene::UpdateCamera(CameraComponent& mainCamera, Time deltaTime)
	{
		using namespace Input;

		if (m_CameraEditorMode == true)
			return;

		Mouse::SetInputMode(Mouse::Hidden);
		const glm::vec2 rotationVector = Mouse::GetNormalizedCursor();
		SetOrientation(mainCamera, rotationVector.x, rotationVector.y);
		Mouse::SetCursorInCenterOfWindow();

		const auto& cameraOrientation = mainCamera.Orientation;

		const int horizontalAxis = Key::GetAxisValue(Key::D, Key::A);
		mainCamera.Position += horizontalAxis * deltaTime.GetSeconds() * glm::normalize(glm::cross(cameraOrientation, Vector3::Up()));

		const int verticalAxis = Key::GetAxisValue(Key::W, Key::S);
		mainCamera.Position += verticalAxis * deltaTime.GetSeconds() * cameraOrientation;
	}

	void Scene::OnKeyPressedEvent(const KeyPressedEvent& e)
    {
		if (e.RepeatCount != 0 || e.KeyCode != Input::Key::C)
            return;

		Entity mainCameraEntity = GetActiveCameraEntity();
		if (!mainCameraEntity)
			return;

        if (m_CameraEditorMode == false)
        {
            m_CameraEditorMode = true;
            Input::Mouse::SetInputMode(Input::Mouse::Visible);

            LOG_TRACE("Switched to Editor Mode");
            return;
        }

        const glm::vec2 rotationVector = Input::Mouse::GetNormalizedCursor();
        SetOrientation(mainCameraEntity.GetComponent<CameraComponent>(), rotationVector.x, rotationVector.y);

        Input::Mouse::SetCursorInCenterOfWindow();

        m_CameraEditorMode = false;
        LOG_TRACE("Switched to Camera Mode");
    }
}
