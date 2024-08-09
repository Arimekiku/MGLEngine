#pragma once

#include "Events/Event.h"

namespace RenderingEngine
{
    class Layer
    {
    public:
        Layer() = default;
        virtual ~Layer() = default;

        virtual void Attach() = 0;
        virtual void Detach() = 0;

        virtual void EveryUpdate() = 0;
    };
}
