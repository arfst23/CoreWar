//******************************************************************************

#include "visualizer.h"
#include "address.h"
#include "redcode.h"

//******************************************************************************

int main()
{
  Visualizer visualizer;
  visualizer.init();
  for (int uid = 0; uid < Redcode::users; uid++)
  {
    int address = uid * 2 * Redcode::width + 16;
    for (int i = 0; i < 32; i++)
      visualizer.set(address + i, uid);
  }
  visualizer.check();

  int delay = 128;
  Address adr(6 * Redcode::width);
  for (;;)
  {
    visualizer.setexec(adr());
    adr.add(1);

    if (visualizer.check())
      break;
  }
  return 0;
}

//******************************************************************************
