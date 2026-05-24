#pragma once

#include "ECS/Registry.hpp"

namespace Vex
{
    using EntityID = uint32_t;

    class Entity final
    {
      public:
        Entity(EntityID id, Weak<Ent::Registry> registry) : m_ID(id), m_Registry(std::move(registry)) {}

        template <typename T>
        void AddComponent(const T& component)
        {
            m_Registry->AddComponent<T>(m_ID, component);
        }

        template <typename T, typename... Args>
        void AddComponent(Args&&... args)
        {
            m_Registry->AddComponent<T>(m_ID, std::forward<Args>(args)...);
        }

        template <typename T>
        void RemoveComponent()
        {
            m_Registry->RemoveComponent<T>(m_ID);
        }

        template <typename T>
        T& GetComponent()
        {
            return m_Registry->GetComponent<T>(m_ID);
        }

        template <typename T>
        const T& GetComponent() const
        {
            return m_Registry->GetComponent<T>(m_ID);
        }

        template <typename T>
        void HasComponent() const
        {
            return m_Registry->HasComponent<T>(m_ID);
        }

        EntityID ID() const { return m_ID; }

      private:
        EntityID m_ID = 0;
        Weak<Ent::Registry> m_Registry = nullptr;
    };
} // namespace Vex
