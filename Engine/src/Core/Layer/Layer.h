#pragma once

#include "Core/Time.h"

namespace Engine
{
    class Layer
    {
    public:
        Layer() = default;
        virtual ~Layer() = default;

        virtual void OnEveryUpdate(Time deltaTime) { }
        virtual void OnGuiUpdate() { }
    };
}
