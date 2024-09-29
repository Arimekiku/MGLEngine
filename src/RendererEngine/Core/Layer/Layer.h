#pragma once

#include "RendererEngine/Core/Time.h"
#include "RendererEngine/Events/Event.h"

namespace RenderingEngine
{
    class Layer
    {
    public:
        Layer() = default;
        virtual ~Layer() = default;

        virtual void Attach() { }
        virtual void Detach() { }
        virtual void OnEveryUpdate(Time deltaTime) { }
        virtual void OnGuiUpdate() { }

        virtual void OnEvent(Event& event) { }
    };
}
