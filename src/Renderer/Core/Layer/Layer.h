#pragma once

#include "Renderer/Events/Event.h"

namespace RenderingEngine
{
    class Layer
    {
    public:
        Layer() = default;
        virtual ~Layer() = default;

        virtual void Attach() { }
        virtual void Detach() { }
        virtual void OnEveryUpdate() { }
        virtual void OnGuiUpdate() { }

        virtual void OnEvent(Event& event) { }
    };
}
