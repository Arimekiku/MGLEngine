#pragma once

#include "Renderer/Core/Layer/Layer.h"

namespace RenderingEngine
{
    class GuiLayer : public Layer
    {
    public:
        GuiLayer() = default;
        ~GuiLayer() override = default;

        void Attach() override;
        void Detach() override;

        static void Begin();
        static void End();

    private:
        float m_Time = 1 / 60.f;
    };
}
