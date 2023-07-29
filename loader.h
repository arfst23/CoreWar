//******************************************************************************

#ifndef LOADER
#define LOADER

#include "memory.h"

class Loader
{
  int len;
  Cell memory[Redcode::length];

public:

  Loader()
    : len(0)
  { }

  void load(const char *name);

  int length() const
  {
    return len;
  }

  const Cell *program() const
  {
    return memory;
  }
};

#endif

//******************************************************************************
