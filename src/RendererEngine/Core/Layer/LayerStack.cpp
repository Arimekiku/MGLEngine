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

    void LayerStack::Pop(const Layer* layer)
    {
        const auto iterator = std::find(m_Layers.begin(), m_Layers.end(), layer);

        if (iterator != m_Layers.end())
            m_Layers.erase(iterator);
    }
}
