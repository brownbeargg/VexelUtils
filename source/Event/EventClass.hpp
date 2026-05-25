#pragma once

#include "Vexel/Utils/Base.hpp"
#include "Vexel/Utils/Memory.hpp"

namespace Vex
{
    enum class EventType
    {
        None = 0,

        AppTicked = VEX_BIT(1),
        AppUpdated = VEX_BIT(2),
        AppRendered = VEX_BIT(3),

        WindowClosed = VEX_BIT(4),
        WindowResized = VEX_BIT(5),
        WindowFocused = VEX_BIT(6),
        WindowLostFocus = VEX_BIT(7),
        WindowMoved = VEX_BIT(8),

        KeyPressed = VEX_BIT(9),
        KeyReleased = VEX_BIT(10),

        MouseButtonPressed = VEX_BIT(11),
        MouseButtonReleased = VEX_BIT(12),
        MouseMoved = VEX_BIT(13),
        MouseScrolled = VEX_BIT(14)
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
