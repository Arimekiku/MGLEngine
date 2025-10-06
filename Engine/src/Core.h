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
    	Ptr(T* ptr) : m_Ptr(ptr) {}
    	Ptr(const std::shared_ptr<T>& ptr) : m_Ptr(ptr) {}

        Ptr(const Ptr& other) : m_Ptr(other.m_Ptr) {}

        template<typename U>
    	Ptr(const Ptr<U>& other) : m_Ptr(std::static_pointer_cast<T>(other.Get())) {}

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
        std::shared_ptr<T> m_Ptr = nullptr;
    };

    struct PtrHash
	{
        template<typename T>
        std::size_t operator()(const Ptr<T>& p) const noexcept
    	{
            return std::hash<T*>()(p.GetRaw());
        }
    };

    struct PtrEqual
	{
        template<typename T>
        bool operator()(const Ptr<T>& a, const Ptr<T>& b) const noexcept
    	{
            return a.GetRaw() == b.GetRaw();
        }
    };
}
