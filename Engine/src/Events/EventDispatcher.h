#pragma once

#include "Core.h"

#include <functional>
#include <typeindex>

#define BIND_FUNC(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Engine
{
	class EventDispatcher
	{
		template <typename TEvent>
		using EventFunc = std::function<void(const TEvent&)>;

    public:
        template<typename TEvent>
        void Subscribe(EventFunc<TEvent> listener)
		{
            auto& vec = m_Listeners[typeid(TEvent)];

            vec.push_back([fn = std::move(listener)](const void* e) 
            {
                fn(*static_cast<const TEvent*>(e));
            });
        }

        template<typename TEvent>
        void Publish(const TEvent& event) const
		{
            if (m_Listeners.contains(typeid(TEvent)) == false)
                return;

            const auto it = m_Listeners.find(typeid(TEvent));
            for (auto& fn : it->second)
                fn(&event);
        }

        template<typename TEvent>
        void Clear()
		{
        	m_Listeners.erase(typeid(TEvent));
        }

        void ClearAll()
		{
            m_Listeners.clear();
        }

    private:
        std::unordered_map<std::type_index, std::vector<std::function<void(const void*)>>> m_Listeners;
	};
}
