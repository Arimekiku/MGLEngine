#pragma once

#include "Core/Layer/Layer.h"

namespace RenderingEngine
{
    class GuiLayer final : public Layer
    {
    public:
        GuiLayer() = default;
        ~GuiLayer() override = default;

        void Attach() override;
        void Detach() override;
        void EveryUpdate() override;

        static void Begin();
        static void End();

    private:
        float m_Time = 1 / 60.f;
    };
}
