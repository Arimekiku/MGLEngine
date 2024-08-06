#include "mxpch.h"
#include "GuiLayer.h"

#include <imgui.h>
#include "Core/Bootstrapper.h"

#include <backends/imgui_impl_opengl3.h>

namespace RenderingEngine
{
    void GuiLayer::Attach()
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void GuiLayer::Detach()
    {
    }

    void GuiLayer::EveryUpdate()
    {
        ImGuiIO& io = ImGui::GetIO();
        const Bootstrapper& boot = Bootstrapper::GetInstance();
        io.DisplaySize = ImVec2(boot.GetWindow().GetWidth(), boot.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time;
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void GuiLayer::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MousePressedEvent>(BIND_FUN(OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseReleasedEvent>(BIND_FUN(OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_FUN(OnMouseMoveEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_FUN(OnMouseScrollEvent));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_FUN(OnWindowResizeEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(BIND_FUN(OnKeyReleasedEvent));
        dispatcher.Dispatch<KeyPressedEvent>(BIND_FUN(OnKeyPressedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(BIND_FUN(OnKeyEvent));
    }

    bool GuiLayer::OnMouseButtonPressedEvent(MousePressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = true;

        return true;
    }

    bool GuiLayer::OnMouseButtonReleasedEvent(MouseReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = false;

        return true;
    }

    bool GuiLayer::OnMouseMoveEvent(MouseMovedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.GetX(), e.GetY());

        return true;
    }

    bool GuiLayer::OnMouseScrollEvent(MouseScrolledEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += e.GetXOffset();
        io.MouseWheel += e.GetYOffset();

        return true;
    }

    bool GuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.f, 1.f);
        glViewport(0, 0, e.GetWidth(), e.GetHeight());

        return true;
    }

    bool GuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = false;

        return true;
    }

    bool GuiLayer::OnKeyEvent(KeyEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();

        int code = e.GetKeyCode();
        if (code > 0 && code < 0x10000)
            io.AddInputCharacter(static_cast<uint16_t>(code));

        return true;
    }

    bool GuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_RIGHT_CONTROL] || io.KeysDown[GLFW_KEY_LEFT_CONTROL];
        io.KeyAlt = io.KeysDown[GLFW_KEY_RIGHT_ALT] || io.KeysDown[GLFW_KEY_LEFT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_RIGHT_SUPER] || io.KeysDown[GLFW_KEY_LEFT_SUPER];
        io.KeyShift = io.KeysDown[GLFW_KEY_RIGHT_SHIFT] || io.KeysDown[GLFW_KEY_LEFT_SHIFT];

        return true;
    }
}
