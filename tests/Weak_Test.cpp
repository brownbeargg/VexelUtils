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

static Vex::Ref<A> a = Vex::CreateRef<A>(69);

TEST(Weak, WeakCountGoesUpAndDown)
{
    {
        Vex::Weak<A> b = Vex::Weak<A>(a);
        EXPECT_EQ(b->GetWeakCount(), 1);
        EXPECT_EQ(a->GetWeakCount(), 1);
        EXPECT_EQ(b->GetRefCount(), 1);
        EXPECT_EQ(a->GetRefCount(), 1);
    }

    EXPECT_EQ(a->GetWeakCount(), 0);
}

TEST(Weak, Resets)
{
    Vex::Weak<A> b = Vex::Weak(a);
    b.Reset(new A(420));
    EXPECT_EQ(b->val, 420);
    EXPECT_EQ(b->val, 420);
}

TEST(Weak, Locks)
{
    Vex::Weak<A> b = Vex::Weak(a);
    Vex::Ref<A> c = b.Lock();
    EXPECT_EQ(a->GetRefCount(), 2);
    EXPECT_EQ(a->GetWeakCount(), 1);
    EXPECT_EQ(a->GetTotalPtrCount(), 3);
}
