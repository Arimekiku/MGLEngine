#pragma once

#include "Core/Logger/Log.h"
#include "Core/Time.h"

#define BIT(x) (1 << (x))
#define NAME_OF(x) (#x)

namespace Engine
{
    template <typename T>
    using Scope = std::unique_ptr<T>;

    /*
    class Ref<T>
    {
    public:
        static T 

    private:
        std::shared_ptr<T> m_Ptr;
    };*/
    template <typename T>
    using Ref = std::shared_ptr<T>;
}
