#pragma once

#include "Vexel/Utils/Base.hpp"
#include "Vexel/Utils/STL.hpp"

namespace Vex
{
    struct MemoryControlBlock final
    {
        uint16_t RefCount = 0;
        uint16_t ScopeCount = 0;
        uint16_t WeakCount = 0;
    };

    struct RefCount
    {
        RefCount(const RefCount& other) {}
        RefCount(RefCount&& rhs) noexcept { VEX_ASSERT(false, "Moving referenced object is unsafe"); }
        RefCount& operator=(const RefCount& other) { return *this; }

        RefCount& operator=(RefCount&& rhs)
        {
            VEX_ASSERT(false, "Moving referenced object is unsafe");
            return *this;
        }

        uint16_t GetRefCount() { return m_ControlBlock.RefCount; }
        uint16_t GetScopeCount() { return m_ControlBlock.ScopeCount; }
        uint16_t GetWeakCount() { return m_ControlBlock.WeakCount; }

        uint32_t GetTotalPtrCount()
        {
            return m_ControlBlock.RefCount + m_ControlBlock.ScopeCount + m_ControlBlock.WeakCount;
        }

      protected:
        RefCount() = default;
        virtual ~RefCount() = default;

      private:
        void IncRefCount() { ++m_ControlBlock.RefCount; }
        void DecRefCount() { --m_ControlBlock.RefCount; }

        void IncScopeCount() { ++m_ControlBlock.ScopeCount; }
        void DecScopeCount() { --m_ControlBlock.ScopeCount; }

        void IncWeakCount() { ++m_ControlBlock.WeakCount; }
        void DecWeakCount() { --m_ControlBlock.WeakCount; }

      private:
        mutable MemoryControlBlock m_ControlBlock{};

        template <typename T>
        friend class Ref;
        template <typename T>
        friend class Scope;
        template <typename T>
        friend class Weak;
    };
} // namespace Vex
