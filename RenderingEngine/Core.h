#pragma once

#define BIT(x) (1 << (x))
#define NAME_OF(x) (#x)

#define BIND_FUN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace RenderingEngine
{
    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T>
    using Ref = std::shared_ptr<T>;
}
