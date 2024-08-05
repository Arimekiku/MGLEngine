#pragma once

#include "Events/Event.h"

namespace RenderingEngine
{
    class Layer
    {
    public:
        Layer() = default;
        virtual ~Layer() = default;

        virtual void Attach()
        {
        }

        virtual void Detach()
        {
        }

        virtual void EveryUpdate()
        {
        }

        virtual void OnEvent(Event& e)
        {
        }
    };
}
