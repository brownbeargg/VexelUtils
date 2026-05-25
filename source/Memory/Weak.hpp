#pragma once

#include "Memory/ControlBlock.hpp"

namespace Vex
{
    template <typename T>
    class Ref;

    template <typename T>
    class Scope;

    template <typename T>
    class Weak final
    {
      public:
        Weak() = default;
        ~Weak() { Destroy(); }

        // --------------------------------------------------------------------------------
        // Parameterized constructors
        // --------------------------------------------------------------------------------

        Weak(T* pData) : m_pData(pData), m_pControl(new ControlBlock) {}
        Weak(nullptr_t pData) {}

        // --------------------------------------------------------------------------------
        // Copy/move constructors/assignent operators
        // --------------------------------------------------------------------------------

        Weak(const Weak<T>& other) : m_pData(other.m_pData), m_pControl(other.m_pControl)
        {
            m_pControl->IncWeakCount();
        }

        Weak(Weak<T>&& rhs) : m_pData(rhs.m_pData), m_pControl(rhs.m_pControl)
        {
            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;
        }

        Weak<T>& operator=(const Weak<T>& other)
        {
            Destroy();

            m_pData = other.m_pData;

            m_pControl = other.m_pControl;
            m_pControl->IncWeakCount();

            return *this;
        }

        Weak<T>& operator=(Weak<T>&& rhs)
        {
            if (this == &rhs)
                return *this;

            Destroy();

            m_pData = rhs.m_pData;
            m_pControl = rhs.m_pControl;

            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;

            return *this;
        }

        // --------------------------------------------------------------------------------
        // Convertible pointer types
        // --------------------------------------------------------------------------------

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Weak(const Weak<U>& other) : m_pData(other.m_pData), m_pControl(other.m_pControl)
        {
            m_pControl->IncWeakCount();
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Weak(Weak<U>&& rhs) : m_pData(rhs.m_pData), m_pControl(rhs.m_pControl)
        {
            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Weak<T>& operator=(const Weak<U>& other)
        {
            Destroy();

            m_pData = other.m_pData;

            m_pControl = other.m_pControl;
            m_pControl->IncWeakCount();

            return *this;
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Weak<T>& operator=(Weak<U>&& rhs)
        {
            if (this == (Weak<T>)&rhs)
                return;

            Destroy();

            m_pData = rhs.m_pData;
            m_pControl = rhs.m_pControl;

            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;

            return *this;
        }

        // ---------------------------------------------------------------------------------
        // Refs
        // ---------------------------------------------------------------------------------

        Weak(const Ref<T>& other) : m_pData(other.m_pData), m_pControl(other.m_pControl)
        {
            m_pControl->IncWeakCount();
        }

        Weak(Ref<T>&& rhs) : m_pData(rhs.m_pData), m_pControl(rhs.m_pControl)
        {
            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;
        }

        Weak<T>& operator=(const Ref<T>& other)
        {
            Destroy();

            m_pData = other.m_pData;

            m_pControl = other.m_pControl;
            m_pControl->IncWeakCount();

            return *this;
        }

        Weak<T>& operator=(Ref<T>&& rhs)
        {
            if (this == (Weak<T>*)&rhs)
                return *this;

            Destroy();

            m_pData = rhs.m_pData;
            m_pControl = rhs.m_pControl;

            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;

            return *this;
        }

        // ---------------------------------------------------------------------------------
        // Scopes
        // ---------------------------------------------------------------------------------

        Weak(const Scope<T>& other) : m_pData(other.m_pData), m_pControl(other.m_pControl)
        {
            m_pControl->IncWeakCount();
        }

        Weak(Scope<T>&& rhs) : m_pData(rhs.m_pData), m_pControl(rhs.m_pControl)
        {
            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;
        }

        Weak<T>& operator=(const Scope<T>& other)
        {
            Destroy();

            m_pData = other.m_pData;

            m_pControl = other.m_pControl;
            m_pControl->IncWeakCount();

            return *this;
        }

        Weak<T>& operator=(Scope<T>&& rhs)
        {
            if (this == (Weak<T>*)&rhs)
                return *this;

            Destroy();

            m_pData = rhs.m_pData;
            m_pControl = rhs.m_pControl;

            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;

            return *this;
        }

        // --------------------------------------------------------------------------------
        // Operators
        // --------------------------------------------------------------------------------

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

        Ref<T> Lock() { return Ref<T>(*this); }
        bool Expired() { return m_pControl->GetRefCount() || m_pControl->GetScopeCount(); }

        Weak<T>& Reset(T* pData)
        {
            if (m_pData == pData)
                return *this;

            Destroy();

            if (pData)
            {
                m_pData = pData;
                m_pControl = new ControlBlock;
            }

            return *this;
        }

        void Destroy()
        {
            m_pData = nullptr;

            if (m_pControl)
                m_pControl->DecWeakCount();

            m_pControl = nullptr;
        }

        const T* Get() const { return m_pData; }
        T* Get() { return m_pData; }

        uint16_t GetRefCount() { return m_pControl->RefCount; }
        uint16_t GetScopeCount() { return m_pControl->ScopeCount; }
        uint16_t GetWeakCount() { return m_pControl->WeakCount; }

        uint32_t GetTotalPtrCount()
        {
            return m_pControl->RefCount + m_pControl->ScopeCount + m_pControl->WeakCount;
        }

        template <typename... Args>
        static Weak<T> Create(Args&&... args)
        {
            return Weak<T>(new T(std::forward<Args>(args)...));
        }

        static Weak<T> Borrow(T* pData) { return Weak<T>(pData).ShouldOwn(false); }

      private:
        Weak<T>& ShouldOwn(bool shouldOwn)
        {
            m_pControl->HasOwnership = shouldOwn;
            return *this;
        }

      private:
        T* m_pData = nullptr;
        ControlBlock* m_pControl;

        template <typename U>
        friend class Ref;

        template <typename U>
        friend class Scope;

        template <typename U>
        friend class Weak;
    };
} // namespace Vex
