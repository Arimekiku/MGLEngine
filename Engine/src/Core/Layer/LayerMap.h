#pragma once

#include "Core.h"
#include "Layer.h"

#include <typeindex>

namespace Engine
{
    template<typename TLayer>
    concept DerivedFromLayer = std::is_base_of_v<Layer, TLayer>;

    class LayerMap
    {
    public:
        LayerMap() = default;
        ~LayerMap() = default;

		template<typename TLayer> requires DerivedFromLayer<TLayer>
        void Push(const Ref<TLayer>& layer)
        {
            m_Layers.try_emplace(typeid(TLayer), layer);
        }

        template<typename TLayer> requires DerivedFromLayer<TLayer>
        void Pop(const Ref<TLayer>& layer)
        {
            if (m_Layers.contains(typeid(TLayer)) == false)
            {
                LOG_ERROR("Can't remove requested layer with id {0}", typeid(layer).name());
                return;
            }

            m_Layers.erase(typeid(TLayer));
        }

        template<typename TLayer>
        Ref<TLayer> Get()
        {
            if (m_Layers.contains(typeid(TLayer)) == false)
            {
                LOG_ERROR("Can't retrieve layer with id {0}", typeid(TLayer).name());
                return nullptr;
            }

            return std::dynamic_pointer_cast<TLayer>(m_Layers[typeid(TLayer)]);
        }

        std::unordered_map<std::type_index, Ref<Layer>>::iterator begin() { return m_Layers.begin(); }
        std::unordered_map<std::type_index, Ref<Layer>>::iterator end() { return m_Layers.end(); }

    private:
        std::unordered_map<std::type_index, Ref<Layer>> m_Layers;
    };
}

