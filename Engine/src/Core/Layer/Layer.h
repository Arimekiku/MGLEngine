#pragma once

#include "Core/Time.h"
#include "Events/Event.h"

namespace Engine
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
