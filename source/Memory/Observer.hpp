#pragma once

namespace Vex
{
    template <typename T>
    class Ref;

    template <typename T>
    class Scope;

    template <typename T>
    class Weak;

    template <typename T>
    class Observer
    {
      public:
        Observer() = default;
        ~Observer() = default;

        // --------------------------------------------------------------------------------
        // Parameterized constructors
        // --------------------------------------------------------------------------------

        Observer(T* pData) : m_pData(pData) {}
        Observer(nullptr_t pData) {}

        // --------------------------------------------------------------------------------
        // Copy/move constructors/assignent operators
        // --------------------------------------------------------------------------------

        Observer(const Observer<T>& other) : m_pData(other.m_pData) {}

        Observer(Observer<T>&& rhs) : m_pData(rhs.m_pData) { rhs.m_pData = nullptr; }

        Observer<T>& operator=(const Observer<T>& other)
        {
            m_pData = other.m_pData;
            return *this;
        }

        Observer<T>& operator=(Observer<T>&& rhs)
        {
            if (this == &rhs)
                return *this;

            m_pData = rhs.m_pData;
            rhs.m_pData = nullptr;

            return *this;
        }

        // --------------------------------------------------------------------------------
        // Convertible pointer types
        // --------------------------------------------------------------------------------

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Observer(const Observer<U>& other) : m_pData(other.m_pData)
        {
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Observer(Observer<U>&& rhs) : m_pData(rhs.m_pData)
        {
            rhs.m_pData = nullptr;
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Observer<T>& operator=(const Observer<U>& other)
        {
            m_pData = other.m_pData;
            return *this;
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Observer<T>& operator=(Observer<U>&& rhs)
        {
            if (this == (Observer<T>)&rhs)
                return *this;

            m_pData = rhs.m_pData;
            rhs.m_pData = nullptr;

            return *this;
        }

        // --------------------------------------------------------------------------------
        // Operators
        // --------------------------------------------------------------------------------

        bool operator==(const T* pData) const { return pData == m_pData; }
        bool operator==(const Observer<T>& other) const { return other.m_pData == m_pData; }

        bool operator==(const Ref<T>& other) const { return other.m_pData == m_pData; }
        bool operator==(const Scope<T>& other) const { return other.m_pData == m_pData; }
        bool operator==(const Weak<T>& other) const { return other.m_pData == m_pData; }

        const T& operator*() const { return *m_pData; }
        T& operator*() { return *m_pData; }
        const T* operator->() const { return m_pData; }
        T* operator->() { return m_pData; }

        operator bool() const { return m_pData; }

        // --------------------------------------------------------------------------------
        // Methods
        // --------------------------------------------------------------------------------

        const T* Get() const { return m_pData; }
        T* Get() { return m_pData; }

        static Observer<T> Borrow(T* pData) { return Observer<T>(pData); }

      private:
        T* m_pData = nullptr;

        template <typename U>
        friend class Observer;

        template <typename U>
        friend class Ref;

        template <typename U>
        friend class Scope;

        template <typename U>
        friend class Weak;
    };
} // namespace Vex
