#pragma once

#include "Core.h"
#include "Layer.h"

namespace Engine
{
    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();

        void Push(const Ref<Layer>& layer);
        void Pop(const Ref<Layer>& layer);

        std::vector<Ref<Layer>>::iterator begin() { return m_Layers.begin(); }
        std::vector<Ref<Layer>>::iterator end() { return m_Layers.end(); }

    private:
        std::vector<Ref<Layer>> m_Layers;
    };
}

