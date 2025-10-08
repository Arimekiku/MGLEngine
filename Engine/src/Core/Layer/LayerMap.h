#pragma once

#include <any>
#include <typeindex>

#include "Layer.h"
#include "Core/Logger/Log.h"

namespace Engine
{
    template<typename TLayer>
    concept DerivedFromLayer = std::is_base_of_v<Layer, TLayer>;

    class LayerMap
    {
    public:
        LayerMap() = default;
        ~LayerMap() { m_Layers.clear(); }

		template<typename TLayer> requires DerivedFromLayer<TLayer>
        void Push(TLayer* layer)
        {
            m_Layers.try_emplace(typeid(TLayer), layer);
        }

        template<typename TLayer> requires DerivedFromLayer<TLayer>
        void Pop(const TLayer* layer)
        {
            if (m_Layers.contains(typeid(TLayer)) == false)
            {
                LOG_ERROR("Can't remove requested layer with id {0}", typeid(layer).name());
                return;
            }

            m_Layers.erase(typeid(TLayer));
        }

        template<typename TLayer>
        TLayer* Get()
        {
            if (m_Layers.contains(typeid(TLayer)) == false)
            {
                LOG_ERROR("Can't retrieve layer with id {0}", typeid(TLayer).name());
                return nullptr;
            }

            return dynamic_cast<TLayer*>(m_Layers[typeid(TLayer)]);
        }

        std::unordered_map<std::type_index, Layer*>::iterator begin() { return m_Layers.begin(); }
        std::unordered_map<std::type_index, Layer*>::iterator end() { return m_Layers.end(); }

    private:
        std::unordered_map<std::type_index, Layer*> m_Layers;
    };
}

