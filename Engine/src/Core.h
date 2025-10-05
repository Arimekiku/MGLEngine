#pragma once

#include <memory>
#include <utility>

namespace Engine
{
    template <typename T>
    class Ptr
    {
    public:
        Ptr() = default;
        Ptr(std::nullptr_t) : m_Ptr(nullptr) {}
        Ptr(const std::shared_ptr<T>& ptr) : m_Ptr(ptr) {}
		Ptr(T* ptr) : m_Ptr(ptr) {}
        Ptr(const Ptr& other) = default;
        Ptr(Ptr&& other) noexcept = default;
        ~Ptr() = default;

        std::shared_ptr<T>& Get() { return m_Ptr; }
        const std::shared_ptr<T>& Get() const { return m_Ptr; }
		T* GetRaw() const { return m_Ptr.get(); }
        size_t UseCount() const { return m_Ptr.use_count(); }
        bool Unique() const { return m_Ptr.unique(); }

        template <typename... Args>
        static Ptr Create(Args&&... args)
        {
            return Ptr(new T(std::forward<Args>(args)...));
        }

        Ptr& operator=(const Ptr& other) = default;
        Ptr& operator=(Ptr&& other) noexcept = default;
        T* operator->() const { return m_Ptr.get(); }
        T& operator*() const { return *m_Ptr; }
        explicit operator bool() const { return static_cast<bool>(m_Ptr); }

    private:
        std::shared_ptr<T> m_Ptr;
    };
}
