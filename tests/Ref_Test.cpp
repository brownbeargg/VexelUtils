#include <gtest/gtest.h>

#include <Vexel/Utils.hpp>

class A : public Vex::RefCount
{
  public:
    int val = 0;

  public:
    A() = default;
    A(int val) : val(val) {}
};

template class Vex::Ref<A>;

class B : public A
{
  public:
    int val = 0;

  public:
    B() = default;
    B(int val) : val(val) {}
};

template class Vex::Ref<B>;

TEST(Ref, StartsWithOneRef)
{
    Vex::Ref<A> a = Vex::Ref<A>::Create(5);
    EXPECT_EQ(a->GetTotalPtrCount(), 1);
    EXPECT_EQ(a->GetRefCount(), 1);
}

static Vex::Ref<A> a = Vex::Ref<A>::Create(69);

TEST(Ref, RefCountGoesUpAndDown)
{
    {
        Vex::Ref<A> b = a;
        EXPECT_EQ(b->GetRefCount(), 2);
        EXPECT_EQ(a->GetRefCount(), 2);
    }

    EXPECT_EQ(a->GetRefCount(), 1);
}

TEST(Ref, Resets)
{
    Vex::Ref<A> b = a;
    b.Reset(new A(420));
    EXPECT_EQ(b->val, 420);
    EXPECT_EQ(b->val, 420);
}

TEST(Ref, Inheritance)
{
    EXPECT_NO_THROW(Vex::Ref<A> b = Vex::Ref<B>::Create());
}
