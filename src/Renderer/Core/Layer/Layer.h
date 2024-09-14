#pragma once

#include "Renderer/Events/Event.h"

namespace RenderingEngine
{
    class Layer
    {
    public:
        Layer() = default;
        virtual ~Layer() = default;

        virtual void Attach()
        {
        };

        virtual void Detach()
        {
        };

        virtual void EveryUpdate() = 0;

        virtual void OnEvent(Event& event)
        {
        }
    };
}
