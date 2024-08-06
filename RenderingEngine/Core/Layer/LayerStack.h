#pragma once

#include "Core/Layer.h"

namespace RenderingEngine
{
    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();

        void Push(Layer* layer);
        void Pop(const Layer* layer);

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }

    private:
        std::vector<Layer*> m_Layers;
    };
}

