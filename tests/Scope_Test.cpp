#include <gtest/gtest.h>

#include <Vexel/Utils.hpp>

class A
{
  public:
    int val = 0;

  public:
    A() = default;
    A(int val) : val(val) {}
};

template class Vex::Scope<A>;
