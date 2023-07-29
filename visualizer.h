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

  enum
  {
    hscale = 4,
    vscale = 3,
    offset = 10,
    
    maxdelay = 16384,
    mindelay = 1,
    defdelay = 512,
  };

  Graphix *graphix;
  int delay;

public:

  Visualizer();
  ~Visualizer();

  void init();

  void set(int address, int uid);
  void setexec(int address);

  void flush();
  bool check();
  bool wait();
};

#endif

//******************************************************************************
