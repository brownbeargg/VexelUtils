#pragma once

#include "Memory/ControlBlock.hpp"

namespace Vex
{
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

        explicit Scope(T* pData) { Reset(pData); }
        Scope(nullptr_t pData) { Reset(pData); }

        Scope(const Scope<T>& other) { Reset(other.m_pData); }
        Scope(Scope<T>&& rhs) { Move<T>(std::move(rhs)); }
        Scope<T>& operator=(const Scope<T>& other) { return Reset(other.m_pData); }
        Scope<T>& operator=(Scope<T>&& rhs) { return Move<T>(std::move(rhs)); }

        // --------------------------------------------------------------------------------
        // Convertible types
        // --------------------------------------------------------------------------------

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Scope(const Scope<U>& other)
        {
            Reset(other.m_Data);
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Scope(Scope<U>&& rhs)
        {
            Move<U>(std::move(rhs));
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Scope<T>& operator=(const Scope<U>& other)
        {
            return Reset(other.m_Data);
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Scope<T>& operator=(Scope<U>&& rhs)
        {
            return Move<T>(std::move(rhs));
        }

        // --------------------------------------------------------------------------------
        // Refs
        // --------------------------------------------------------------------------------

        Scope(const Ref<T>& other) { Reset(other.m_pData); }
        Scope(Ref<T>&& rhs) { Move<T>(std::move(rhs)); }
        Scope<T>& operator=(const Ref<T>& other) { return Reset(other.m_pData); }
        Scope<T>& operator=(Ref<T>&& rhs) { return Move<T>(std::move(rhs)); }

        // --------------------------------------------------------------------------------
        // Weaks
        // --------------------------------------------------------------------------------

        Scope(const Weak<T>& other) { Reset(other.m_pData); }
        Scope(Weak<T>&& rhs) { Move<T>(std::move(rhs)); }
        Scope<T>& operator=(const Weak<T> other) { return Reset(other.m_pData); }
        Scope<T> operator=(Weak<T>&& rhs) { return Move<T>(std::move(rhs)); }

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

        Scope<T>& Reset(T* pData);
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
        static Scope<T> Create(Args&&... args)
        {
            return Scope<T>(new T(std::forward<Args>(args)...));
        }

      private:
        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Scope<T>& Move(Scope<U>&& rhs);

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
    // Scope function definitions
    // --------------------------------------------------------------------------------

    template <typename T>
    Scope<T>& Scope<T>::Reset(T* pData)
    {
        if (m_pData == pData)
            return *this;

        Destroy();

        if (pData)
        {
            m_pData = pData;

            m_pControlBlock = ControlBlock::Get(pData);
            m_pControlBlock->IncScopeCount();
        }

        return *this;
    }

    template <typename T>
    void Scope<T>::Destroy()
    {
        if (!m_pData)
            return;

        if (!m_pControlBlock->DecScopeCount())
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
    Scope<T>& Scope<T>::Move(Scope<U>&& rhs)
    {
        if (this == (Scope<T>*)&rhs)
            return *this;

        Destroy();

        m_pData = rhs.m_pData;
        rhs.m_pData = nullptr;

        m_pControlBlock = rhs.m_pControlBlock;
        rhs.m_pControlBlock = nullptr;

        return *this;
    }
} // namespace Vex
