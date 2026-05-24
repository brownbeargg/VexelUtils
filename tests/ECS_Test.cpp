#include <gtest/gtest.h>

#include <Vexel/Utils.hpp>

struct PositionComponent
{
    float X = 0;
    float Y = 0;

    PositionComponent(float x, float y) : X(x), Y(y) {}
    PositionComponent& operator=(const PositionComponent&) = default;
    bool operator==(const PositionComponent& other) const { return X == other.X && Y == other.Y; }
};

TEST(ECS, AddsAndGetsAndRemoves)
{
    Vex::Ent::Registry reg;
    Vex::Entity entity = {reg.CreateEntity(), Vex::Weak(&reg)};

    entity.AddComponent<PositionComponent>(10, 3);
    EXPECT_EQ(entity.GetComponent<PositionComponent>(), PositionComponent(10, 3));

    entity.GetComponent<PositionComponent>() = {5, 2};
    EXPECT_EQ(entity.GetComponent<PositionComponent>(), PositionComponent(5, 2));

    entity.RemoveComponent<PositionComponent>();
}
