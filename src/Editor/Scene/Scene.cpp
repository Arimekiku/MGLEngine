#include "Scene.h"
#include "Entity.h"

#include <glm/trigonometric.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace RenderingEngine
{
	static void OnDirectionalLightAdded(entt::registry& registry, entt::entity entity)
	{
		DirectLightComponent component = registry.get<DirectLightComponent>(entity);

        const auto& defaultMaterial = MaterialImporter::GetMaterial("Default");
		defaultMaterial->BindMat4Uniform("u_lightViewProj", component.GetDLMatrix());
	}

	void SetOrientation(CameraComponent& mainCamera, const float rotX, const float rotY)
    {
		glm::vec3& cameraOrientation = mainCamera.Orientation;

        // Calculates upcoming vertical change in the Orientation
        const glm::vec3 newOrientation = glm::rotate(cameraOrientation, glm::radians(-rotX),
                                                     glm::normalize(glm::cross(cameraOrientation, Vector3::Up())));

        // Decides whether or not the next vertical Orientation is legal or not
        if (std::abs(glm::angle(newOrientation, Vector3::Up()) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            cameraOrientation = newOrientation;
        }

        // Rotates the Orientation left and right
        cameraOrientation = glm::rotate(cameraOrientation, glm::radians(-rotY), Vector3::Up());
    }

	void Scene::UpdateCamera(CameraComponent& mainCamera, Time deltaTime)
	{
		using namespace Input; 

		if (m_CameraEditorMode == true)
		{
            return;
		}

        Mouse::SetInputMode(Mouse::HIDDEN);
        const glm::vec2 rotationVector = Mouse::GetNormalizedCursor();
        SetOrientation(mainCamera, rotationVector.x, rotationVector.y);
        Mouse::SetCursorInCenterOfWindow();

		glm::vec3& cameraOrientation = mainCamera.Orientation;

		if (Key::Pressed(Key::A))
		{
            mainCamera.Position += deltaTime.GetSeconds() * -glm::normalize(glm::cross(cameraOrientation, {0, 1, 0}));
		}

		if (Key::Pressed(Key::D))
		{
            mainCamera.Position += deltaTime.GetSeconds() * glm::normalize(glm::cross(cameraOrientation, {0, 1, 0}));
		}

		if (Key::Pressed(Key::W))
		{
            mainCamera.Position += deltaTime.GetSeconds() * cameraOrientation;
		}

		if (Key::Pressed(Key::S))
		{
            mainCamera.Position += deltaTime.GetSeconds() * -cameraOrientation;
		}
	}

	Scene::Scene()
	{
		m_Entities.on_construct<DirectLightComponent>().connect<&OnDirectionalLightAdded>();
	}

	Entity Scene::Instantiate(const std::string& name)
	{
		Entity entity = Entity(m_Entities.create(), this);

		entity.AddComponent<NameComponent>(name.empty() ? "Entity" : name);

		return entity;
	}

	void Scene::Destroy(Entity entity)
	{
		m_Entities.destroy(entity);
	}

	Entity Scene::GetActiveCameraEntity()
	{
		auto view = m_Entities.view<CameraComponent>();
		for (auto& cameraEntity : view)
		{
			CameraComponent& camera = view.get<CameraComponent>(cameraEntity);

			if (camera.Enabled == false)
			{
				continue;
			}

			return Entity(cameraEntity, this);
		}

		return Entity();
	}

	void Scene::OnEveryUpdate(Time deltaTime)
	{
		Entity mainCameraEntity = GetActiveCameraEntity();
		if (!mainCameraEntity)
		{
			return;
		}

		UpdateCamera(mainCameraEntity.GetComponent<CameraComponent>(), deltaTime);
	}

	void Scene::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(BIND_FUNC(OnKeyPressedEvent));
	}

	bool Scene::OnKeyPressedEvent(const KeyPressedEvent& e)
    {
		if (e.GetRepeatCount() != 0)
		{
            return true;
		}

        if (e.GetKeyCode() == Input::Key::C)
        {
			Entity mainCameraEntity = GetActiveCameraEntity();
			if (!mainCameraEntity)
			{
				return true;
			}

            if (m_CameraEditorMode == false)
            {
                m_CameraEditorMode = true;
                Input::Mouse::SetInputMode(Input::Mouse::VISIBLE);

                LOG_TRACE("Switched to Editor Mode");
                return true;
            }

            const glm::vec2 rotationVector = Input::Mouse::GetNormalizedCursor();
            SetOrientation(mainCameraEntity.GetComponent<CameraComponent>(), rotationVector.x, rotationVector.y);

            Input::Mouse::SetCursorInCenterOfWindow();

            m_CameraEditorMode = false;
            LOG_TRACE("Switched to Camera Mode");
        }

        return true;
    }
}