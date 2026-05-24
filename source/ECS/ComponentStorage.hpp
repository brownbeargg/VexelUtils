#pragma once

namespace Vex
{
    using EntityID = uint32_t;

    class IComponentStorage
    {
      public:
        virtual ~IComponentStorage() = default;

        virtual void Remove(EntityID entity) = 0;
    };

    template <typename T>
    class ComponentStorage : public IComponentStorage
    {
      public:
        void Add(EntityID entity, const T& component);

        void Remove(EntityID entity) override { m_Components.erase(entity); }

        bool Has(EntityID entity) const { return m_Components.contains(entity); }

        T& Get(EntityID entity) { return m_Components.at(entity); }
        const T& Get(EntityID entity) const { return m_Components.at(entity); }

      private:
        std::map<EntityID, T> m_Components;
    };

    template <typename T>
    void ComponentStorage<T>::Add(EntityID entity, const T& component)
    {
        m_Components.insert_or_assign(entity, component);
    }
} // namespace Vex
