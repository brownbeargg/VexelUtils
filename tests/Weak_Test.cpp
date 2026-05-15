#include <gtest/gtest.h>

#include <Vexel/Memory.hpp>

class A : public Vex::RefCount
{
  public:
    int val = 0;

  public:
    A() = default;
    A(int val) : val(val) {}
};

template class Vex::Weak<A>;

TEST(Weak, StartsWithOneRef)
{
    Vex::Weak<A> a = Vex::CreateWeak<A>(5);
    EXPECT_EQ(a->GetTotalPtrCount(), 1);
    EXPECT_EQ(a->GetWeakCount(), 1);
}

Vex::Weak<A> a = Vex::CreateWeak<A>(69);

TEST(Weak, WeakCountGoesUpAndDown)
{
    {
        Vex::Weak<A> b = a;
        EXPECT_EQ(b->GetWeakCount(), 2);
        EXPECT_EQ(a->GetWeakCount(), 2);
    }

    EXPECT_EQ(a->GetWeakCount(), 1);
}

TEST(Weak, Resets)
{
    Vex::Weak<A> b = a;
    b.Reset(new A(420));
    EXPECT_EQ(b->val, 420);
    EXPECT_EQ(b->val, 420);
}

TEST(Weak, Locks)
{
    Vex::Ref<A> b = a.Lock();
    EXPECT_EQ(a->GetRefCount(), 1);
    EXPECT_EQ(a->GetWeakCount(), 1);
    // EXPECT_EQ(a->GetTotalPtrCount(), 2);
}
