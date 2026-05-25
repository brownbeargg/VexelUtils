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

        explicit Weak(T* pData) { Reset(pData); }
        Weak(nullptr_t pData) { Reset(pData); }

        Weak(const Weak<T>& other) { Reset(other.m_pData); }
        Weak(Weak<T>&& rhs) { Move<T>(std::move(rhs)); }
        Weak<T>& operator=(const Weak<T>& other) { return Reset(other.m_pData); }
        Weak<T>& operator=(Weak<T>&& rhs) { return Move<T>(std::move(rhs)); }

        // --------------------------------------------------------------------------------
        // Convertible types
        // --------------------------------------------------------------------------------

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Weak(const Weak<U>& other)
        {
            Reset(other.m_Data);
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Weak(Weak<U>&& rhs)
        {
            Move<U>(std::move(rhs));
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Weak<T>& operator=(const Weak<U>& other)
        {
            return Reset(other.m_Data);
        }

        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Weak<T>& operator=(Weak<U>&& rhs)
        {
            return Move<U>(std::move(rhs));
        }

        // --------------------------------------------------------------------------------
        // Refs
        // --------------------------------------------------------------------------------

        Weak(const Ref<T>& other) { Reset(other.m_pData); }
        Weak(Ref<T>&& rhs) { Move<T>(std::move(rhs)); }
        Weak<T>& operator=(const Ref<T>& other) { return Reset(other.m_pData); }
        Weak<T>& operator=(Ref<T>&& rhs) { return Move<T>(std::move(rhs)); }

        // --------------------------------------------------------------------------------
        // Scopes
        // --------------------------------------------------------------------------------

        Weak(const Scope<T>& other) { Reset(other.m_pData); }
        Weak(Scope<T>&& rhs) { Move<T>(std::move(rhs)); }
        Weak<T>& operator=(const Scope<T>& other) { return Reset(other.m_pData); }
        Weak<T>& operator=(Scope<T>&& rhs) { return Move<T>(std::move(rhs)); }

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

        Ref<T> Lock() { return Ref<T>(m_pData); }

        Weak<T>& Reset(T* pData);
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
        static Weak<T> Create(Args&&... args)
        {
            return Weak<T>(new T(std::forward<Args>(args)...));
        }

      private:
        template <typename U>
            requires std::is_convertible_v<U*, T*>
        Weak<T>& Move(Weak<U>&& rhs);

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

    template <typename T>
    Weak<T>& Weak<T>::Reset(T* pData)
    {
        if (m_pData == pData)
            return *this;

        Destroy();

        if (pData)
        {
            m_pData = pData;

            m_pControlBlock = ControlBlock::Get(pData);
            m_pControlBlock->IncWeakCount();
        }

        return *this;
    }

    template <typename T>
    void Weak<T>::Destroy()
    {
        if (!m_pData)
            return;

        m_pControlBlock->DecWeakCount();

        m_pData = nullptr;
        m_pControlBlock = nullptr;
    }

    template <typename T>
    template <typename U>
        requires std::is_convertible_v<U*, T*>
    Weak<T>& Weak<T>::Move(Weak<U>&& rhs)
    {
        if (this == (Weak<T>*)&rhs)
            return *this;

        Destroy();

        m_pData = rhs.m_pData;
        rhs.m_pData = nullptr;

        m_pControlBlock = rhs.m_pControlBlock;
        rhs.m_pControlBlock = nullptr;

        return *this;
    }
} // namespace Vex
