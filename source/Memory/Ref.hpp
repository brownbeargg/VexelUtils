#pragma once

#include "RefCount.hpp"

namespace Vex
{
    template <typename T>
    class Weak;

    template <typename T>
    class Ref final
    {
      public:
        Ref() = default;
        ~Ref() { Destroy(); }

        explicit Ref(T* data) { Reset(data); }

        Ref(const Ref<T>& other) { Reset(other.m_Data); }
        Ref(Ref&& rhs) { Move(std::move(rhs)); }
        Ref<T>& operator=(const Ref<T>& other) { return Reset(other.m_Data); }
        Ref<T>& operator=(Ref<T>&& rhs) { return Move(std::move(rhs)); }

        explicit Ref(const Weak<T>& other) { Reset(other.m_Data); }
        explicit Ref(Weak<T>&& rhs) { Move(Ref<T>(rhs)); }
        Ref<T>& operator=(const Weak<T>& other) { return Reset(other.m_Data); }
        Ref<T>& operator=(Weak<T>&& rhs) { return Move(Ref<T>(rhs)); }

        const T& operator*() const { return *m_Data; }
        T& operator*() { return *m_Data; }
        const T* operator->() const { return m_Data; }
        T* operator->() { return m_Data; }

        operator bool() const { return m_Data; }

        Ref<T>& Reset(T* data);
        void Destroy();

        const T* Get() const { return m_Data; }
        T* Get() { return m_Data; }

      private:
        Ref<T>& Move(Ref<T>&& rhs);

      private:
        T* m_Data = nullptr;

        template <typename U>
        friend class Weak;
    };

    template <typename T>
    Ref<T>& Ref<T>::Reset(T* data)
    {
        Destroy();

        m_Data = data;
        if (m_Data)
            m_Data->IncRefCount();

        return *this;
    }

    template <typename T>
    void Ref<T>::Destroy()
    {
        if (!m_Data)
            return;

        m_Data->DecRefCount();
        if (m_Data->GetRefCount() == 0)
            delete m_Data;

        m_Data = nullptr;
    }

    template <typename T>
    Ref<T>& Ref<T>::Move(Ref<T>&& rhs)
    {
        if (this == &rhs)
            return *this;

        Destroy();

        m_Data = rhs.m_Data;
        rhs.m_Data = nullptr;

        return *this;
    }

    template <typename T, typename... Args>
    Ref<T> CreateRef(Args&&... args)
    {
        return Ref<T>(new T(std::forward<Args>(args)...));
    }
} // namespace Vex

#include "Weak.hpp"
