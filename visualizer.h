//******************************************************************************

#ifndef VISUALIZER
#define VISUALIZER
#include "redcode.h"
#include <stdint.h>

//******************************************************************************

class Graphix;

class Visualizer
{
private:

  Graphix *graphix;
  int delay;

public:

  Visualizer();
  ~Visualizer();

  void init();

  void set(int address, int uid);
  void setexec(int address, int uid);

  void flush();
  bool check();
  bool wait();
};

#endif

//******************************************************************************
