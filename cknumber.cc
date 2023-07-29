//******************************************************************************

#include "number.h"
#include "assert.h"

//******************************************************************************

int main()
{
  {
    Number n1 = 0;
    assert(n1() == 0);
    Number n2 = 1;
    assert(n2() == 1);
    Number n3 = -1;
    assert(n3() == -1);
    Number n4 = Redcode::range - 1;
    assert(n4() == Redcode::range - 1);
    Number n5 = -Redcode::range + 1;
    assert(n5() == -Redcode::range + 1);
    Number n6 = Redcode::range;
    assert(n6() == Redcode::range - 1);
    Number n7 = -Redcode::range;
    assert(n7() == -Redcode::range + 1);
  } 

  {
    Number n1 = 2;
    Number n2 = 1;
    n2.sub(n1);
    assert(n2() == -1);
    Number n3 = -1;
    n3.add(n1);
    assert(n3() == 1);
    Number n4 = 1;
    Number n5 = -1;
    Number n6 = Redcode::range - 1;
    n6.add(n4);
    assert(n6() == Redcode::range - 1);
    n6.sub(n5);
    assert(n6() == Redcode::range - 1);
    Number n7 = -Redcode::range + 1;
    n7.sub(n4);
    assert(n7() == -Redcode::range + 1);
    n7.add(n5);
    assert(n7() == -Redcode::range + 1);
  }

  {
    Number n1 = 1;
    Number n2 = -1;
    assert(n1.cmp(n1) == 0);
    assert(n1.cmp(n2) == 1);
    assert(n2.cmp(n1) == -1);
  }

  return 0;
}

//******************************************************************************
