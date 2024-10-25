#include "SceneLayer.h"
#include "ComponentRegistry.h"
#include "Entity.h"

#include <imgui.h>

namespace RenderingEngine
{
    SceneLayer::SceneLayer() : m_Camera(glm::vec3(0, 0, 10))
    {
        m_GuiRenderer.SetContext(m_Scene);

        //CAMERAS
        Entity camera = m_Scene->Instantiate("Camera");
        camera.AddComponent<CameraComponent>(true);

        //MATERIALS AND LIGHT
        const auto& lightShader = std::make_shared<Shader>(
			RESOURCES_PATH "Shaders/areaLight.vert",
            RESOURCES_PATH "Shaders/areaLight.frag");

		lightShader->Bind();
		lightShader->BindUniformFloat3("u_LightColor", {1, 1, 1});

		const auto& lightMaterial = std::make_shared<Material>(lightShader);

        Entity areaLight = m_Scene->Instantiate("AreaLight");
        auto& areaLightTransform = areaLight.GetComponent<TransformComponent>();
        auto& lightComponent = areaLight.AddComponent<AreaLightComponent>();
        areaLight.AddComponent<MeshComponent>(MeshImporter::CreateCube());
        areaLight.AddComponent<MaterialComponent>(lightMaterial);

        areaLightTransform.Position = glm::vec3(0, 5, -5);

        const auto& m_FaceTexture = std::make_shared<Texture>(RESOURCES_PATH "Images/face.png");
        const auto& m_HouseTexture = std::make_shared<Texture>(RESOURCES_PATH "Images/house.png");

        Entity directLight = m_Scene->Instantiate("DirectLight");
        auto& dirLightComp = directLight.AddComponent<DirectLightComponent>();

		const auto& shader = std::make_shared<Shader>(
            RESOURCES_PATH "Shaders/default.vert",
            RESOURCES_PATH "Shaders/default.frag");

        shader->Bind();
        shader->BindUniformInt1("u_Texture", 0);
		shader->BindUniformInt1("u_DepthMap", 1);
        shader->BindUniformFloat3("u_LightColor", lightComponent.Color);
        shader->BindUniformFloat3("u_LightPos", areaLightTransform.Position);
		shader->BindUniformMat4("u_lightViewProj", dirLightComp.GetDLMatrix());

        auto& m_DefaultMaterial = std::make_shared<Material>(shader);
        m_DefaultMaterial->SetTextureMap(m_HouseTexture);

        //GROUND
        const auto& groundPlane = MeshImporter::CreatePlane(50.0f);
        Entity plane = m_Scene->Instantiate("Ground");
        plane.AddComponent<MeshComponent>(groundPlane);
        plane.AddComponent<MaterialComponent>(m_DefaultMaterial);

        auto& transformComponent = plane.GetComponent<TransformComponent>();
        transformComponent.Position = glm::vec3(0, -10, 0);
        transformComponent.Rotation = glm::vec3(-90, 0, 0);

        //BASEBALL BAT
        const auto& baseballMesh = MeshImporter::CreateMesh(RESOURCES_PATH "Models/baseballbat_mesh.fbx");
        Entity baseballBat = m_Scene->Instantiate("Bat");
        baseballBat.AddComponent<MeshComponent>(baseballMesh);
        baseballBat.AddComponent<MaterialComponent>(m_DefaultMaterial);
        baseballBat.GetComponent<TransformComponent>().Position = glm::vec3(10, 6, 3);

        //PRIMITIVES
        const auto& planeMesh = MeshImporter::CreatePlane(1.0f);
        Entity planePrimitive = m_Scene->Instantiate("Plane");
        planePrimitive.AddComponent<MeshComponent>(planeMesh);
        planePrimitive.AddComponent<MaterialComponent>(m_DefaultMaterial);

        const auto& cubeMesh = MeshImporter::CreateCube(1.0f);
        Entity cubePrimitive = m_Scene->Instantiate("Cube");
        cubePrimitive.AddComponent<MeshComponent>(cubeMesh);
        cubePrimitive.AddComponent<MaterialComponent>(m_DefaultMaterial);
        cubePrimitive.GetComponent<TransformComponent>().Position = glm::vec3(0, 0, 2);

        const auto& sphereMesh = MeshImporter::CreateSphere(1.0f);
        Entity spherePrimitive = m_Scene->Instantiate("Sphere");
        spherePrimitive.AddComponent<MeshComponent>(sphereMesh);
        spherePrimitive.AddComponent<MaterialComponent>(m_DefaultMaterial);
        spherePrimitive.GetComponent<TransformComponent>().Position = glm::vec3(0, 0, 5);
    }

    void SceneLayer::OnEveryUpdate(const Time deltaTime)
    {
        m_LastTime = deltaTime;

        Renderer::OnEveryUpdate(m_Camera);
        m_Camera.EveryUpdate(deltaTime);

        m_Scene->OnEveryUpdate(deltaTime);
    }

    void SceneLayer::OnGuiUpdate()
    {
        static bool docking = true;
        static bool optFullscreen = true;
        static ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_None;

        ImGuiDockNodeFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (optFullscreen == true)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

            windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (dockFlags & ImGuiDockNodeFlags_PassthruCentralNode)
            windowFlags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Renderer", &docking, windowFlags);
        ImGui::PopStyleVar();

        if (optFullscreen)
            ImGui::PopStyleVar(2);

        const ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const ImGuiID dockID = ImGui::GetID("DockSpace");
            ImGui::DockSpace(dockID, ImVec2(0, 0), dockFlags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Close"))
                {
                    docking = false;
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Begin("Parameters");

        ImGui::Text("DeltaTime: %.3f (%.0fmil)", m_LastTime.GetSeconds(), m_LastTime.GetMilliseconds());

        ImGui::End();

        m_GuiRenderer.DrawViewport();
        m_GuiRenderer.DrawScenePanel();
        m_GuiRenderer.DrawInspectorPanel();
        //m_GuiRenderer.DrawGuizmos();

        ImGui::End();
    }

    void SceneLayer::OnEvent(Event& event)
    {
        m_Camera.OnEvent(event);
    }
}
