#pragma once

namespace Vex
{
    template <typename T>
    class Ref;

    template <typename T>
    class Weak;

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
            return Move(std::move(rhs));
        }

        explicit Scope(const Ref<T>& other) { Reset(other.m_Data); }
        explicit Scope(Ref<T>&& rhs) { Move(Scope<T>(rhs)); }
        Scope<T>& operator=(const Ref<T>& other) { return Reset(other.m_Data); }
        Scope<T>& operator=(Ref<T>&& rhs) { return Move(Scope<T>(rhs)); }

        explicit Scope(const Weak<T>& other) { Reset(other.m_Data); }
        explicit Scope(Weak<T>&& rhs) { Move(Scope<T>(rhs)); }
        Scope<T>& operator=(const Weak<T>& other) { return Reset(other.m_Data); }
        Scope<T>& operator=(Weak<T>&& rhs) { return Move(Scope<T>(rhs)); }

        bool operator==(Ref<T> other) { return m_Data == other.m_Data; }
        bool operator==(Scope<T> other) { return m_Data == other.m_Data; }
        bool operator==(Weak<T> other) { return m_Data == other.m_Data; }

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

        template <typename U>
        friend class Ref;

        template <typename U>
        friend class Scope;

        template <typename U>
        friend class Weak;
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
