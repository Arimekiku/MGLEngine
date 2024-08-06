#pragma once

#include "Core/Layer.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

namespace RenderingEngine
{
    class GuiLayer : public Layer
    {
    public:
        GuiLayer() = default;
        ~GuiLayer() override = default;

        void Attach() override;
        void Detach() override;
        void EveryUpdate() override;
        void OnEvent(Event& e) override;

    private:
        bool OnMouseButtonPressedEvent(MousePressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseReleasedEvent& e);
        bool OnMouseMoveEvent(MouseMovedEvent& e);
        bool OnMouseScrollEvent(MouseScrolledEvent& e);
        bool OnWindowResizeEvent(WindowResizeEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        bool OnKeyEvent(KeyEvent& e);
        bool OnKeyPressedEvent(KeyPressedEvent& e);

        float m_Time = 1 / 60.f;
    };
}
