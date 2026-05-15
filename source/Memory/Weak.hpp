#pragma once

#include "Ref.hpp"
#include "RefCount.hpp"

namespace Vex
{
    template <typename T>
    class Ref;

    template <typename T>
    class Weak final
    {
      public:
        Weak() = default;
        ~Weak() { Destroy(); }

        explicit Weak(T* data) { Reset(data); }

        Weak(const Weak<T>& other) { Reset(other.m_Data); }
        Weak(Weak<T>&& rhs) { Move(std::move(rhs)); }
        Weak<T>& operator=(const Weak<T>& other) { return Reset(other.m_Data); }
        Weak<T>& operator=(Weak<T>&& rhs) { return Move(std::move(rhs)); }

        explicit Weak(const Ref<T>& other) { Reset(other.m_Data); }
        explicit Weak(Ref<T>&& rhs) { Move(Weak<T>(rhs)); }
        Weak<T>& operator=(const Ref<T>& other) { return Reset(other.m_Data); }
        Weak<T>& operator=(Ref<T>&& rhs) { return Move(Weak<T>(rhs)); }

        const T& operator*() const { return *m_Data; }
        T& operator*() { return *m_Data; }
        const T* operator->() const { return m_Data; }
        T* operator->() { return m_Data; }

        operator bool() const { return m_Data; }

        Weak<T>& Reset(T* data);
        void Destroy();

        bool Expired() { return m_Data->GetRefCount(); }
        Ref<T> Lock() { return Ref<T>(m_Data); }

        T* Get() { return m_Data; }
        const T* Get() const { return m_Data; }

      private:
        Weak<T>& Move(Weak<T>&& rhs);

      private:
        T* m_Data = nullptr;

        template <typename U>
        friend class Ref;
    };

    template <typename T>
    Weak<T>& Weak<T>::Reset(T* data)
    {
        Destroy();

        if (data)
        {
            m_Data = data;
            m_Data->IncWeakCount();
        }

        return *this;
    }

    template <typename T>
    void Weak<T>::Destroy()
    {
        if (!m_Data)
            return;

        m_Data->DecWeakCount();

        m_Data = nullptr;
    }

    template <typename T>
    Weak<T>& Weak<T>::Move(Weak<T>&& rhs)
    {
        if (this == &rhs)
            return *this;

        Destroy();

        m_Data = rhs.m_Data;
        rhs.m_Data = nullptr;

        return *this;
    }

    template <typename T, typename... Args>
    Weak<T> CreateWeak(Args&&... args)
    {
        return Weak<T>(new T(std::forward<Args>(args)...));
    }
} // namespace Vex
