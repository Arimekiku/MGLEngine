#pragma once

#include "Core/Layer.h"

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
        float m_Time = 1 / 60.f;
    };
}
