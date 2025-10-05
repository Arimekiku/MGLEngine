#include "GuiLayer.h"
#include "Core/Bootstrapper.h"

#include <backends/imgui_impl_opengl3.cpp> 
#include <backends/imgui_impl_glfw.cpp>
#include <ImGuizmo.h>

namespace Engine
{
    GuiLayer::GuiLayer(const Ref<Window>& window)
    {
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(window->GetNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 450");
        m_Parent = window;
    }

	GuiLayer::~GuiLayer()
	{
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

    void GuiLayer::Begin() const
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void GuiLayer::End() const
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(m_Parent->GetWidth(), m_Parent->GetHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            const auto window = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(window);
        }
    }
}
