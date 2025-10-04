#include "LayerStack.h"

namespace Engine
{
    LayerStack::~LayerStack()
    {
        m_Layers.clear();
    }

    void LayerStack::Push(const Ref<Layer>& layer)
    {
        m_Layers.emplace_back(layer);
    }

    void LayerStack::Pop(const Ref<Layer>& layer)
    {
        const auto iterator = std::ranges::find(m_Layers, layer);
        if (iterator == m_Layers.end())
            return;

        m_Layers.erase(iterator);
    }
}
