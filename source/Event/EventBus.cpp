#include "EventBus.hpp"

#include <Event/EventClass.hpp>

namespace Vex
{
    void EventBus::Queue(Scope<Event> event)
    {
        m_Queue.push_back(std::move(event));
    }

    void EventBus::Dispatch()
    {
        for (Scope<Event>& event : m_Queue)
        {
            auto it = m_Observers.find(event->GetType());

            if (it == m_Observers.end())
                continue;

            for (Callback<Event>& callback : it->second)
                callback(*event);
        }

        ClearQueue();
    }

    void EventBus::DispatchImidiate(Scope<Event> event)
    {
        auto it = m_Observers.find(event->GetType());

        if (it == m_Observers.end())
            return;

        for (Callback<Event>& callback : it->second)
            callback(*event);
    }
} // namespace Vex
