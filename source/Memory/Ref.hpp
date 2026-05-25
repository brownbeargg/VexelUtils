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

        explicit Ref(T* pData) { Reset(pData); }
        Ref(nullptr_t pData) { Reset(pData); }

        Ref(const Ref<T>& other) { Reset(other.m_pData); }
        Ref(Ref<T>&& rhs) { Move<T>(std::move(rhs)); }
        Ref<T>& operator=(const Ref<T>& other) { return Reset(other.m_pData); }
        Ref<T>& operator=(Ref<T>&& rhs) { return Move<T>(std::move(rhs)); }

        // --------------------------------------------------------------------------------
        // Convertible types
        // --------------------------------------------------------------------------------

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
            Move<U>(std::move(rhs));
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
            return Move<U>(std::move(rhs));
        }

        // --------------------------------------------------------------------------------
        // Scopes
        // --------------------------------------------------------------------------------

        Ref(const Scope<T>& other) { Reset(other.m_pData); }
        Ref(Scope<T>&& rhs) { Move<T>(std::move(rhs)); }
        Ref<T>& operator=(const Scope<T>& other) { return Reset(other.m_pData); }
        Ref<T>& operator=(Scope<T>&& rhs) { return Move<T>(std::move(rhs)); }

        // --------------------------------------------------------------------------------
        // Weaks
        // --------------------------------------------------------------------------------

        Ref(const Weak<T>& other) { Reset(other.m_pData); }
        Ref(Weak<T>&& rhs) { Move<T>(std::move(rhs)); }
        Ref<T>& operator=(const Weak<T> other) { return Reset(other.m_pData); }
        Ref<T> operator=(Weak<T>&& rhs) { return Move<T>(std::move(rhs)); }

        // --------------------------------------------------------------------------------
        // Operators
        // --------------------------------------------------------------------------------

        bool operator==(const Ref<T>& other) const { return m_pData == other.m_pData; }
        bool operator==(const Scope<T>& other) const { return m_pData == other.m_pData; }
        bool operator==(const Weak<T>& other) const { return m_pData == other.m_pData; }

        const T& operator*() const { return *m_pData; }
        T& operator*() { return *m_pData; }
        const T* operator->() const { return m_pData; }
        T* operator->() { return m_pData; }

        operator bool() const { return m_pData; }

        // --------------------------------------------------------------------------------
        // Member functions
        // --------------------------------------------------------------------------------

        Ref<T>& Reset(T* pData);
        void Destroy();

        const T* Get() const { return m_pData; }
        T* Get() { return m_pData; }

        uint16_t GetRefCount() { return m_pControlBlock->RefCount; }
        uint16_t GetScopeCount() { return m_pControlBlock->ScopeCount; }
        uint16_t GetWeakCount() { return m_pControlBlock->WeakCount; }

        uint32_t GetTotalPtrCount()
        {
            return m_pControlBlock->RefCount + m_pControlBlock->ScopeCount + m_pControlBlock->WeakCount;
        }

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
        T* m_pData = nullptr;
        ControlBlock* m_pControlBlock = nullptr;

        template <typename U>
        friend class Ref;

        template <typename U>
        friend class Scope;

        template <typename U>
        friend class Weak;
    };

    // --------------------------------------------------------------------------------
    // Ref function definitions
    // --------------------------------------------------------------------------------

    template <typename T>
    Ref<T>& Ref<T>::Reset(T* pData)
    {
        if (m_pData == pData)
            return *this;

        Destroy();

        if (pData)
        {
            m_pData = pData;

            m_pControlBlock = ControlBlock::Get(pData);
            m_pControlBlock->IncRefCount();
        }

        return *this;
    }

    template <typename T>
    void Ref<T>::Destroy()
    {
        if (!m_pData)
            return;

        if (!m_pControlBlock->DecRefCount() && !m_pControlBlock->ScopeCount)
        {
            ControlBlock::Destroy(m_pData);
            delete m_pData;
        }

        m_pData = nullptr;
        m_pControlBlock = nullptr;
    }

    template <typename T>
    template <typename U>
        requires std::is_convertible_v<U*, T*>
    Ref<T>& Ref<T>::Move(Ref<U>&& rhs)
    {
        if (this == (Ref<T>*)&rhs)
            return *this;

        Destroy();

        m_pData = rhs.m_pData;
        rhs.m_pData = nullptr;

        m_pControlBlock = rhs.m_pControlBlock;
        rhs.m_pControlBlock = nullptr;

        return *this;
    }
} // namespace Vex
