#include <gtest/gtest.h>

#include <Vexel/Utils.hpp>

class A
{
  public:
    int Data = 0;
};

template class Vex::Observer<A>;

TEST(Observer, works)
{
    A* obj = new A(5);
    Vex::Ref<A> objRef = Vex::Ref<A>(obj);

    Vex::Observer<A> a;
    a = obj;

    EXPECT_EQ(a == obj && a == objRef.Get(), true);
    a = Vex::Observer<A>::Borrow(objRef.Get());
    EXPECT_EQ(a == obj && a == objRef.Get(), true);
    a = nullptr;
    EXPECT_EQ(a == nullptr, true);
}
