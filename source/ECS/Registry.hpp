#pragma once

#include "ECS/ComponentStorage.hpp"
#include "Vexel/Utils/Memory.hpp"

namespace Vex
{
    namespace Ent
    {
        using EntityID = uint32_t;

        class Registry : public RefCount
        {
          public:
            EntityID CreateEntity() { return m_NextEntity++; }

            void DestroyEntity(EntityID entity)
            {
                for (auto& [type, storage] : m_Storages)
                    storage->Remove(entity);
            }

            template <typename T>
            void AddComponent(EntityID entity, const T& component)
            {
                GetStorage<T>().Add(entity, component);
            }

            template <typename T, typename... Args>
            void AddComponent(EntityID entity, Args&&... args)
            {
                GetStorage<T>().Add(entity, T(std::forward<Args>(args)...));
            }

            template <typename T>
            void RemoveComponent(EntityID entity)
            {
                VEX_ASSERT(HasComponent<T>(entity), "Entity does not have component")
                GetStorage<T>().Remove(entity);
            }

            template <typename T>
            bool HasComponent(EntityID entity) const
            {
                return GetStorage<T>().Has(entity);
            }

            template <typename T>
            T& GetComponent(EntityID entity)
            {
                VEX_ASSERT(HasComponent<T>(entity), "Entity does not have component")
                return GetStorage<T>().Get(entity);
            }

            template <typename T>
            const T& GetComponent(EntityID entity) const
            {
                VEX_ASSERT(HasComponent<T>(entity), "Entity does not have component")
                return GetStorage<T>().Get(entity);
            }

          private:
            template <typename T>
            ComponentStorage<T>& GetStorage()
            {
                std::type_index type = typeid(T);
                if (!m_Storages.contains(type))
                    m_Storages[type] = std::make_unique<ComponentStorage<T>>();

                return static_cast<ComponentStorage<T>&>(*m_Storages.at(type));
            }

            template <typename T>
            const ComponentStorage<T>& GetStorage() const
            {
                std::type_index type = typeid(T);
                return static_cast<const ComponentStorage<T>&>(*m_Storages.at(type));
            }

          private:
            EntityID m_NextEntity = 1;
            std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>> m_Storages;
        };
    } // namespace Ent
} // namespace Vex
