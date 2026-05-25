#pragma once

#include "Memory/ControlBlock.hpp"

namespace Vex
{
    template <typename T>
    class Scope;

    template <typename T>
    class Weak;

    template <typename T>
    class Ref final
    {
      public:
        Ref() = default;
        ~Ref() { Destroy(); }

        // --------------------------------------------------------------------------------
        // Parameterized constructors
        // --------------------------------------------------------------------------------

        Ref(T* pData) : m_pData(pData), m_pControl(new ControlBlock) { m_pControl->IncRefCount(); }
        Ref(nullptr_t pData) {}

        // --------------------------------------------------------------------------------
        // Copy/move constructors/assignent operators
        // --------------------------------------------------------------------------------

        Ref(const Ref<T>& other) : m_pData(other.m_pData), m_pControl(other.m_pControl)
        {
            m_pControl->IncRefCount();
        }

        Ref(Ref<T>&& rhs) : m_pData(rhs.m_pData), m_pControl(rhs.m_pControl)
        {
            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;
        }

        Ref<T>& operator=(const Ref<T>& other)
        {
            Destroy();

            m_pData = other.m_pData;

            m_pControl = other.m_pControl;
            m_pControl->IncRefCount();

            return *this;
        }

        Ref<T>& operator=(Ref<T>&& rhs)
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
        Ref(const Ref<U>& other) : m_pData(other.m_pData), m_pControl(other.m_pControl)
        {
            m_pControl->IncRefCount();
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Ref(Ref<U>&& rhs) : m_pData(rhs.m_pData), m_pControl(rhs.m_pControl)
        {
            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Ref<T>& operator=(const Ref<U>& other)
        {
            Destroy();

            m_pData = other.m_pData;

            m_pControl = other.m_pControl;
            m_pControl->IncRefCount();

            return *this;
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Ref<T>& operator=(Ref<U>&& rhs)
        {
            if (this == (Ref<T>)&rhs)
                return *this;

            Destroy();

            m_pData = rhs.m_pData;
            m_pControl = rhs.m_pControl;

            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;

            return *this;
        }

        // --------------------------------------------------------------------------------
        // Scopes
        // --------------------------------------------------------------------------------

        Ref(const Scope<T>& other) : m_pData(other.m_pData), m_pControl(other.m_pControl)
        {
            m_pControl->IncRefCount();
        }

        Ref(Scope<T>&& rhs) : m_pData(rhs.m_pData), m_pControl(rhs.m_pControl)
        {
            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;
        }

        Ref<T>& operator=(const Scope<T>& other)
        {
            Destroy();

            m_pData = other.m_pData;

            m_pControl = other.m_pControl;
            m_pControl->IncRefCount();

            return *this;
        }

        Ref<T>& operator=(Scope<T>&& rhs)
        {
            if (this == (Ref<T>*)&rhs)
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

        Ref(const Weak<T>& other) : m_pData(other.m_pData), m_pControl(other.m_pControl)
        {
            m_pControl->IncRefCount();
        }

        Ref(Weak<T>&& rhs) : m_pData(rhs.m_pData), m_pControl(rhs.m_pControl)
        {
            rhs.m_pData = nullptr;
            rhs.m_pControl = nullptr;
        }

        Ref<T>& operator=(const Weak<T>& other)
        {
            Destroy();

            m_pData = other.m_pData;

            m_pControl = other.m_pControl;
            m_pControl->IncRefCount();

            return *this;
        }

        Ref<T>& operator=(Weak<T>&& rhs)
        {
            if (this == (Ref<T>*)&rhs)
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

        Ref<T>& Reset(T* pData)
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

            if (!m_pControl->DecRefCount() && !m_pControl->ScopeCount)
            {
                if (m_pControl->HasOwnership)
                    delete m_pData;

                if (!m_pControl->GetWeakCount())
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
        static Ref<T> Create(Args&&... args)
        {
            return Ref<T>(new T(std::forward<Args>(args)...));
        }

        static Ref<T> Borrow(T* pData) { return Ref<T>(pData).ShouldOwn(false); }

      private:
        Ref<T>& ShouldOwn(bool shouldOwn)
        {
            m_pControl->HasOwnership = shouldOwn;
            return *this;
        }

      private:
        T* m_pData = nullptr;
        ControlBlock* m_pControl = nullptr;

        template <typename U>
        friend class Ref;

        template <typename U>
        friend class Scope;

        template <typename U>
        friend class Weak;
    };
} // namespace Vex
