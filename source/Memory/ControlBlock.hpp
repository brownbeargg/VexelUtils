#pragma once

namespace Vex
{
    struct ControlBlock final
    {
        uint16_t RefCount = 0;
        uint16_t ScopeCount = 0;
        uint16_t WeakCount = 0;

        bool HasOwnership = true;

        uint16_t IncRefCount() { return ++RefCount; }
        uint16_t DecRefCount() { return --RefCount; }
        uint16_t GetRefCount() { return RefCount; }

        uint16_t IncScopeCount() { return ++ScopeCount; }
        uint16_t DecScopeCount() { return --ScopeCount; }
        uint16_t GetScopeCount() { return ScopeCount; }

        uint16_t IncWeakCount() { return ++WeakCount; }
        uint16_t DecWeakCount() { return --WeakCount; }
        uint16_t GetWeakCount() { return WeakCount; }

        static ControlBlock* Get(void* address)
        {
            if (!s_ControlBlocks.contains(address))
                s_ControlBlocks[address] = ControlBlock();

            return &s_ControlBlocks[address];
        }

        static void Destroy(void* address) { s_ControlBlocks.erase(address); }

      private:
        static inline std::unordered_map<void*, ControlBlock> s_ControlBlocks{};
    };
} // namespace Vex
