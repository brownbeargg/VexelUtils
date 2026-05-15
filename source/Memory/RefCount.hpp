#pragma once

#include "Vexel/STL.hpp"

namespace Vex
{
    struct MemoryControlBlock final
    {
        uint32_t RefCount = 0;
        uint32_t WeakCount = 0;
    };

    struct RefCount
    {
        RefCount() = default;
        virtual ~RefCount() = default;

        RefCount(const RefCount& other) = delete;
        RefCount(RefCount&& rhs) noexcept = delete;
        RefCount& operator=(const RefCount& other) = delete;
        RefCount& operator=(RefCount&& rhs) noexcept = delete;

        uint32_t GetRefCount() { return m_ControlBlock.RefCount; }
        uint32_t GetWeakCount() { return m_ControlBlock.WeakCount; }

        uint32_t GetTotalPtrCount() { return m_ControlBlock.RefCount + m_ControlBlock.WeakCount; }

      private:
        void IncRefCount() { ++m_ControlBlock.RefCount; }
        void DecRefCount() { --m_ControlBlock.RefCount; }

        void IncWeakCount() { ++m_ControlBlock.WeakCount; }
        void DecWeakCount() { --m_ControlBlock.WeakCount; }

      private:
        mutable MemoryControlBlock m_ControlBlock{};

        template <typename T>
        friend class Ref;
        template <typename T>
        friend class Weak;
    };
} // namespace Vexel
