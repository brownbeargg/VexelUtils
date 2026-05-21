#include <gtest/gtest.h>

#include <Vexel/Utils.hpp>

class TestEvent : public Vex::Event
{
  public:
    int Data;

    VEX_EVENT_CLASS_TYPE(Vex::EventType::AppTicked);
    VEX_EVENT_CLASS_CATEGORY(Vex::EventCategory::App);
};

int testVal = 0;

void onEvent(TestEvent& e)
{
    ++testVal;
}

TEST(Event, Imidiate)
{
    Vex::EventBus eBus;

    eBus.Observe<TestEvent>(VEX_BIND_FN(onEvent));

    eBus.DispatchImidiate(Vex::Scope<TestEvent>::Create());

    EXPECT_EQ(testVal, 1);
}

TEST(Event, Queue)
{
    testVal = 0;

    Vex::EventBus eBus;

    eBus.Observe<TestEvent>(VEX_BIND_FN(onEvent));

    eBus.Queue(Vex::Scope<TestEvent>::Create());

    Vex::Scope<TestEvent> tEvent = Vex::Scope<TestEvent>::Create();
    tEvent->Data = 200;
    eBus.Queue(std::move(tEvent));

    eBus.Dispatch();

    EXPECT_EQ(testVal, 2);
}
