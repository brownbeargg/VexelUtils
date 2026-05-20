#pragma once

#define VEX_EVENT_CLASS_TYPE(type)                                                                           \
    static Vex::EventType Type()                                                                             \
    {                                                                                                        \
        return type;                                                                                         \
    }                                                                                                        \
    Vex::EventType GetType() const override                                                                  \
    {                                                                                                        \
        return type;                                                                                         \
    }

#define VEX_EVENT_CLASS_CATEGORY(category)                                                                   \
    virtual Vex::EventCategory GetCategoryFlags() const override                                             \
    {                                                                                                        \
        return category;                                                                                     \
    }
