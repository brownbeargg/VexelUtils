#pragma once

#define VEX_BIND_FN(fn)                                                                                      \
    [](auto&&... args) -> decltype(auto)                                                                     \
    {                                                                                                        \
        return fn(std::forward<decltype(args)>(args)...);                                                    \
    }

#define VEX_BIND_METHOD(fn)                                                                                  \
    [this](auto&&... args) -> decltype(auto)                                                                 \
    {                                                                                                        \
        return this->fn(std::forward<decltype(args)>(args)...);                                              \
    }

#define VEX_BIT(x) (1 << x)
