#pragma once

#include "Vexel/Utils/Base.hpp"

namespace Vex
{
    enum class EventType
    {
        None = 0,

        AppTick = VEX_BIT(1),
        AppUpdate = VEX_BIT(2),
        AppRender = VEX_BIT(3)
    };

    enum class EventCategory
    {
        None = 0,

        App = VEX_BIT(1),
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
