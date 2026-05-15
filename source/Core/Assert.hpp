#pragma once

#if defined(_MSVC_VER)
    #define VEX_DEBUG_BREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
    #define VEX_DEBUG_BREAK() __builtin_trap()
#else
    #define VEX_DEBUG_BREAK std::abort()
#endif

#if VEX_ENABLE_DEBUG_ASSERTS
    #define VEX_ASSERT(expr, ...)                                                                            \
        do                                                                                                   \
        {                                                                                                    \
            if (!(expr))                                                                                     \
            {                                                                                                \
                VEX_CORE_ERROR(                                                                              \
                    "Assertion Failed with expression: {0}\n message: {1}\n\n file: {2},\n line: {3}",       \
                    #expr, __VA_ARGS__, __FILE__, __LINE__);                                                 \
                VEX_DEBUG_BREAK();                                                                           \
            }                                                                                                \
        } while (0)

    #define VEX_VERIFY(expr, ...)                                                                            \
        do                                                                                                   \
        {                                                                                                    \
            if (!(expr))                                                                                     \
            {                                                                                                \
                VEX_CORE_ERROR(                                                                              \
                    "Verification Failed with expression: {0}\n message: {1}\n\n file: {2},\n line: {3}",    \
                    #expr, __VA_ARGS__, __FILE__, __LINE__);                                                 \
                VEX_DEBUG_BREAK();                                                                           \
            }                                                                                                \
        } while (0)

#else
    #define VEX_ASSERT(expr, ...)
    #define VEX_VERIFY(expr, ...) expr
#endif

#define VEX_RELEASE_ASSERT(expr, ...)                                                                        \
    do                                                                                                       \
    {                                                                                                        \
        if (!(expr))                                                                                         \
        {                                                                                                    \
            VEX_CORE_ERROR(                                                                                  \
                "Assertion Failed with expression: {0}\n message: {1}\n\n file: {2},\n line: {3}", #expr,    \
                __VA_ARGS__, __FILE__, __LINE__);                                                            \
            VEX_DEBUG_BREAK();                                                                               \
            std::abort();                                                                                    \
        }                                                                                                    \
    } while (0)
