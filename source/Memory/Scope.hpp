#pragma once

#include "Memory/ControlBlock.hpp"

namespace Vex
{
    template <typename T>
    class Observer;

    template <typename T>
    class Ref;

    template <typename T>
    class Weak;

    template <typename T>
    class Scope final
    {
      public:
        Scope() = default;
        ~Scope() { Destroy(); }

        // --------------------------------------------------------------------------------
        // Parameterized constructors
        // --------------------------------------------------------------------------------

        explicit Scope(T* pData) : m_pData(pData), m_pControl(new ControlBlock)
        {
            m_pControl->IncScopeCount();
        }
        Scope(nullptr_t pData) {}

        // --------------------------------------------------------------------------------
        // Copy/move constructors/assignent operators
        // --------------------------------------------------------------------------------

        Scope(const Scope<T>& other) : m_pData(other.m_pData), m_pControl(other.m_pControl)
        {
            if (m_pControl)
                m_pControl->IncScopeCount();
        }

        Scope(Scope<T>&& rhs) : m_pData(rhs.m_pData), m_pControl(rhs.m_pControl)
        {
            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;
        }

        Scope<T>& operator=(const Scope<T>& other)
        {
            Destroy();

            m_pData = other.m_pData;

            m_pControl = other.m_pControl;

            if (m_pControl)
                m_pControl->IncScopeCount();

            return *this;
        }

        Scope<T>& operator=(Scope<T>&& rhs)
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
        Scope(const Scope<U>& other) : m_pData(other.m_pData), m_pControl(other.m_pControl)
        {
            if (m_pControl)
                m_pControl->IncScopeCount();
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Scope(Scope<U>&& rhs) : m_pData(rhs.m_pData), m_pControl(rhs.m_pControl)
        {
            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Scope<T>& operator=(const Scope<U>& other)
        {
            Destroy();

            m_pData = other.m_pData;

            m_pControl = other.m_pControl;

            if (m_pControl)
                m_pControl->IncScopeCount();

            return *this;
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Scope<T>& operator=(Scope<U>&& rhs)
        {
            if (this == (Scope<T>)&rhs)
                return *this;

            Destroy();

            m_pData = rhs.m_pData;
            m_pControl = rhs.m_pControl;

            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;

            return *this;
        }

        // --------------------------------------------------------------------------------
        // Refs
        // --------------------------------------------------------------------------------

        Scope(const Ref<T>& other) : m_pData(other.m_pData), m_pControl(other.m_pControl)
        {
            if (m_pControl)
                m_pControl->IncRefCount();
        }

        Scope(Ref<T>&& rhs) : m_pData(rhs.m_pData), m_pControl(rhs.m_pControl)
        {
            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;
        }

        Scope<T>& operator=(const Ref<T>& other)
        {
            Destroy();

            m_pData = other.m_pData;

            m_pControl = other.m_pControl;

            if (m_pControl)
                m_pControl->IncRefCount();

            return *this;
        }

        Scope<T>& operator=(Ref<T>&& rhs)
        {
            if (this == (Scope<T>*)&rhs)
                return *this;

            Destroy();

            m_pData = rhs.m_pData;
            m_pControl = rhs.m_pControl;

            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;

            return *this;
        }

        // --------------------------------------------------------------------------------
        // Weaks
        // --------------------------------------------------------------------------------

        Scope(const Weak<T>& other) : m_pData(other.m_pData), m_pControl(other.m_pControl)
        {
            if (m_pControl)
                m_pControl->IncWeakCount();
        }

        Scope(Weak<T>&& rhs) : m_pData(rhs.m_pData), m_pControl(rhs.m_pControl)
        {
            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;
        }

        Scope<T>& operator=(const Weak<T>& other)
        {
            Destroy();

            m_pData = other.m_pData;

            m_pControl = other.m_pControl;

            if (m_pControl)
                m_pControl->IncWeakCount();

            return *this;
        }

        Scope<T>& operator=(Weak<T>&& rhs)
        {
            if (this == (Scope<T>*)&rhs)
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

        Scope<T>& Reset(T* pData)
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
            if (!m_pData)
                return;

            if (!m_pControl->DecScopeCount())
            {
                if (m_pControl->HasOwnership)
                    delete m_pData;

                if (!m_pControl->GetRefCount() && m_pControl->GetWeakCount())
                    delete m_pControl;
            }

            m_pData = nullptr;
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

        bool IsOwning() { return m_pControl->HasOwnership; }

        template <typename... Args>
        static Scope<T> Create(Args&&... args)
        {
            return Scope<T>(new T(std::forward<Args>(args)...)).ShouldOwn(false);
        }

        static Scope<T> Borrow(T* pData) { return Scope<T>(pData).ShouldOwn(false); }

      private:
        Scope<T>& ShouldOwn(bool shouldOwn)
        {
            m_pControl->HasOwnership = shouldOwn;
            return *this;
        }

      private:
        T* m_pData = nullptr;
        ControlBlock* m_pControl = nullptr;

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
