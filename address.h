//******************************************************************************

#ifndef ADDRESS
#define ADDRESS
#include "number.h"
#include "redcode.h"
#include <algorithm>

//******************************************************************************

class Address
{
  int address;

public:

  Address()
    : address(0)
  { }

  Address(int v)
  {
    address = v % Redcode::size;
    if (address < 0)
      address += Redcode::size;
  }
  
  Address(const Address &other) = default;
  Address& operator=(const Address &other) = default;

  Address& add(const Number num)
  {
    address += num();
    if (address >= Redcode::size)
      address -= Redcode::size;
    if (address < 0)
      address += Redcode::size;
    return *this;
  }

  int dist(const Address &that) const
  {
    int dist1 = address - that.address;
    if (dist1 < 0)
      dist1 += Redcode::size;
    int dist2 = that.address - address;
    if (dist2 < 0)
      dist2 += Redcode::size;
    return std::min(dist1, dist2);
  }

  bool between(const Address &low, const Address &high) const
  {
    return low() <= high()
      ? address >= low() && address <= high()
      : address >= low() || address <= high();
  }

  bool eq(const Address &that) const
  {
    return address == that.address;
  }

  int operator()() const
  {
    return address;
  }
};

#endif

//******************************************************************************
