#pragma once

#include "Vexel/Utils/Base.hpp"

namespace Vex
{
    enum class EventType
    {
        None = 0,

        AppTick = VEX_BIT(1),
        AppUpdate = VEX_BIT(2),
        AppRender = VEX_BIT(3),

        WindowClose = VEX_BIT(4),
        WindowResize = VEX_BIT(5),
        WindowFocus = VEX_BIT(6),
        WindowLostFocus = VEX_BIT(7),
        WindowMoved = VEX_BIT(8),

        KeyPressed = VEX_BIT(9),
        KeyReleased = VEX_BIT(10),

        MouseButtonPressed = VEX_BIT(11),
        MouseButtonReleased = VEX_BIT(12),
        MouseMoved = VEX_BIT(13),
        MouseScrolle = VEX_BIT(14)
    };

    enum class EventCategory
    {
        None = 0,

        App = VEX_BIT(1),
        Input = VEX_BIT(2),
        Keyboard = VEX_BIT(3),
        Mouse = VEX_BIT(4),
    };

    class Event : public RefCount
    {
      public:
        bool Handled = false;

      public:
        virtual EventType GetType() const { return EventType::None; }
        virtual EventCategory GetCategoryFlags() const { return EventCategory::None; }
    };
} // namespace Vex
