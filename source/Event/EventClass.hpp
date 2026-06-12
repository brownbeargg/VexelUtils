#pragma once

#include "Vexel/Utils/Base.hpp"
#include "Vexel/Utils/Memory.hpp"

namespace Vex
{
    enum class EventType
    {
        None = 0,

        AppTicked,
        AppUpdated,
        AppRendered,

        WindowClosed,
        WindowResized,
        WindowFocused,
        WindowLostFocus,
        WindowMoved,

        KeyPressed,
        KeyReleased,

        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    constexpr EventType operator&(EventType a, EventType b)
    {
        return static_cast<EventType>(static_cast<int>(a) & static_cast<int>(b));
    }

    constexpr EventType operator|(EventType a, EventType b)
    {
        return static_cast<EventType>(static_cast<int>(a) | static_cast<int>(b));
    }

    enum class EventCategory
    {
        None = 0,

        App = VEX_BIT(1),
        Input = VEX_BIT(2),
        Keyboard = VEX_BIT(3),
        Mouse = VEX_BIT(4),
    };

    constexpr EventCategory operator&(EventCategory a, EventCategory b)
    {
        return static_cast<EventCategory>(static_cast<int>(a) & static_cast<int>(b));
    }

    constexpr EventCategory operator|(EventCategory a, EventCategory b)
    {
        return static_cast<EventCategory>(static_cast<int>(a) | static_cast<int>(b));
    }

    class Event
    {
      public:
        bool Handled = false;

      public:
        virtual EventType GetType() const { return EventType::None; }
        virtual EventCategory GetCategoryFlags() const { return EventCategory::None; }
    };
} // namespace Vex
