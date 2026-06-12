#pragma once

// --------------------------------------------------------------------------------
// Unsigned ints
// --------------------------------------------------------------------------------

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

constexpr u8 u8_max = std::numeric_limits<u8>::max();
constexpr u16 u16_max = std::numeric_limits<u16>::max();
constexpr u32 u32_max = std::numeric_limits<u32>::max();
constexpr u64 u64_max = std::numeric_limits<u64>::max();

// --------------------------------------------------------------------------------
// Signed ints
// --------------------------------------------------------------------------------

using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

constexpr s8 s8_max = std::numeric_limits<s8>::max();
constexpr s16 s16_max = std::numeric_limits<s16>::max();
constexpr s32 s32_max = std::numeric_limits<s32>::max();
constexpr s64 s64_max = std::numeric_limits<s64>::max();

// --------------------------------------------------------------------------------
// Floating-point types
// --------------------------------------------------------------------------------

using f32 = float;
using f64 = double;
