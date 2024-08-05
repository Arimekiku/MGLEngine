#include "LayerStack.h"

namespace RenderingEngine
{
    LayerStack::~LayerStack()
    {
        for (const auto layer : m_Layers)
            delete layer;
    }

    void LayerStack::Push(Layer* layer)
    {
        m_Layers.emplace_back(layer);
    }

    void LayerStack::Pop(Layer* layer)
    {
        const auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end())
            m_Layers.erase(it);
    }
}
