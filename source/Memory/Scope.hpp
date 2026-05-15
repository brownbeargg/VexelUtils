#pragma once

namespace Vex
{
    template <typename T>
    class Scope
    {
      public:
        Scope() = default;
        ~Scope() { Destroy(); }

        explicit Scope(T* data) { Reset(data); }

        Scope(const Scope<T>&) = delete;
        Scope(Scope<T>&& rhs) { Move(std::move(rhs)); }
        Scope& operator=(const Scope<T>&) = delete;
        Scope& operator=(Scope<T>&& rhs) { return Move(std::move(rhs)); }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Scope(Scope<U>&& rhs)
        {
            Move(std::move(rhs));
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Scope<T>& operator=(Scope<U>&& rhs)
        {
            Move(std::move(rhs));
        }

        const T& operator*() const { return *m_Data; }
        T& operator*() { return *m_Data; }
        const T* operator->() const { return m_Data; }
        T* operator->() { return m_Data; }

        operator bool() const { return m_Data; }

        Scope<T>& Reset(T* data);
        void Destroy();

        T* Get() { return m_Data; }
        const T* Get() const { return m_Data; }

        template <typename... Args>
        static Scope<T> Create(Args&&... args)
        {
            return Scope<T>(new T(std::forward<Args>(args)...));
        }

      private:
        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Scope<T>& Move(Scope<U>&& rhs);

        T* m_Data = nullptr;
    };

    template <typename T>
    Scope<T>& Scope<T>::Reset(T* data)
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
    void Scope<T>::Destroy()
    {
        if (!m_Data)
            return;

        m_Data->DecScopeCount();
        delete m_Data;
        m_Data = nullptr;
    }

    template <typename T>
    template <typename U>
        requires std::is_convertible_v<U*, T*>
    Scope<T>& Scope<T>::Move(Scope<U>&& rhs)
    {
        Destroy();

        m_Data = rhs.m_Data;
        rhs.m_Data = nullptr;

        return *this;
    }
} // namespace Vex
