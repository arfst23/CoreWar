//******************************************************************************

#ifndef NUMBER
#define NUMBER
#include "redcode.h"

//******************************************************************************

class Number
{
  int value;

public:

  Number()
  {
    value = 0;
  }

  Number(int val)
  {
    value = val;
    if (value >= Redcode::range)
      value = Redcode::range - 1;
    if (value <= -Redcode::range)
      value = -Redcode::range + 1;
  }
  
  Number(const Number &that) = default;
  Number& operator=(const Number &that) = default;

  Number add(const Number &that)
  {
    value += that.value;
    if (value >= Redcode::range)
      value = Redcode::range - 1;
    if (value <= -Redcode::range)
      value = -Redcode::range + 1;
    return *this;
  }

  Number sub(const Number &that)
  {
    value -= that.value;
    if (value >= Redcode::range)
      value = Redcode::range - 1;
    if (value <= -Redcode::range)
      value = -Redcode::range + 1;
    return *this;
  }

  int cmp(const Number &that)
  {
    return value < that.value ? -1
      : value > that.value ? 1 : 0;
  }

  int operator()() const
  {
    return value;
  }
};

#endif

//******************************************************************************
