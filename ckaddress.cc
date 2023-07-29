//******************************************************************************

#include "address.h"
#include "number.h"
#include "assert.h"

//******************************************************************************

int main()
{
  {
    Address a1 = 0;
    assert(a1() == 0);
    Address a2 = Redcode::size - 1;
    assert(a2() == Redcode::size - 1);
    Address a3 = -1;
    assert(a3() == Redcode::size - 1);
    Address a4 = Redcode::size;
    assert(a4() == 0);
  }

  {
    Number n1 = 1;
    Number n2 = -1;
    Address a1 = 0;
    a1.add(n1);
    assert(a1() == 1);
    a1.add(n2);
    assert(a1() == 0);
    a1.add(n2);
    assert(a1() == Redcode::size - 1);
    a1.add(n1);
    assert(a1() == 0);
  }

  {
    Address a1 = 0;
    Address a2 = Redcode::size / 2;
    Address a3 = Redcode::size - 1;
    assert(a1.dist(a2) == Redcode::size / 2);
    assert(a1.dist(a3) == 1);
    assert(a2.dist(a1) == Redcode::size / 2);
    assert(a2.dist(a3) == Redcode::size / 2 - 1);
    assert(a3.dist(a1) == 1);
    assert(a3.dist(a2) == Redcode::size / 2 - 1);
  }

  {
    Address a1 = 0;
    Address a2 = 0;
    Address a3 = 1;
    assert(a1.eq(a2));
    assert(!a1.eq(a3));
    assert(!a3.eq(a1));
    Address a4 = 2;
    assert(a1.between(a2, a4));
    assert(a1.between(a4, a2));
    assert(a3.between(a1, a4));
    assert(!a3.between(a4, a1));
  }

  return 0;
}

//******************************************************************************
