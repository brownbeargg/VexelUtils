#pragma once

#include "Vexel/Utils/Memory.hpp"

namespace Vex
{
    class Event;
    enum class EventType;

    class EventBus final
    {
      public:
        template <typename T>
            requires std::derived_from<T, Event>
        using Callback = std::function<void(T&)>;

        template <typename T>
            requires std::derived_from<T, Event>
        void Observe(Callback<T> callback)
        {
            m_Observers[T::Type()].push_back([callback](Event& event) { callback(static_cast<T&>(event)); });
        }

        void Queue(Scope<Event> event);

        void Dispatch();
        void DispatchImidiate(Scope<Event> event);

        void ClearQueue() { m_Queue.clear(); }

      private:
        using EventCallback = std::function<void(Event&)>;

        std::unordered_map<EventType, std::vector<EventCallback>> m_Observers;
        std::vector<Scope<Event>> m_Queue;
    };
} // namespace Vex
