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

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Ref(const Ref<U>& other)
        {
            Reset(other.m_Data);
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Ref(Ref<U>&& rhs)
        {
            Move(std::move(rhs));
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Ref<T>& operator=(const Ref<U>& other)
        {
            return Reset(other.m_Data);
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Ref<T>& operator=(Ref<U>&& rhs)
        {
            return Move(std::move(rhs));
        }

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

        template <typename... Args>
        static Ref<T> Create(Args&&... args)
        {
            return Ref<T>(new T(std::forward<Args>(args)...));
        }

      private:
        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Ref<T>& Move(Ref<U>&& rhs);

      private:
        T* m_Data = nullptr;

        template <typename U>
        friend class Ref;

        template <typename U>
        friend class Weak;
    };

    template <typename T>
    Ref<T>& Ref<T>::Reset(T* data)
    {
        if (m_Data == data)
            return *this;

        Destroy();

        if (data)
        {
            m_Data = data;
            m_Data->IncRefCount();
        }

        return *this;
    }

    template <typename T>
    void Ref<T>::Destroy()
    {
        if (!m_Data)
            return;

        m_Data->DecRefCount();
        if (m_Data->GetRefCount() == 0 && m_Data->GetScopeCount() == 0)
            delete m_Data;

        m_Data = nullptr;
    }

    template <typename T>
    template <typename U>
        requires std::is_convertible_v<U*, T*>
    Ref<T>& Ref<T>::Move(Ref<U>&& rhs)
    {
        Destroy();

        m_Data = rhs.m_Data;
        rhs.m_Data = nullptr;

        return *this;
    }
} // namespace Vex

#include "Weak.hpp"
