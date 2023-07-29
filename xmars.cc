//******************************************************************************

#include "machine.h"
#include "visualizer.h"
#include "redcode.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

//******************************************************************************

int main(int ac, char *av[])
{
  assert(ac == Redcode::users + 1);

  srand(time(NULL));

  Visualizer visualizer;
  Machine<Visualizer> machine(visualizer, av + 1);


  int turns = 0;
  int count[Redcode::users];
  memset(count, 0, sizeof(count));

  for (;;)
  {
    int uid = machine.run();
    if (uid >= 0)
      count[uid]++;
    turns++;

    if (uid == -1) // timeout
      printf("%d: timeout\n", turns);
    else if (uid == -2) // terminate
      printf("%d: all terminated\n", turns);
    else if (uid == -3) // shutdown
      printf("%d: user shutdown\n", turns);
    else
      printf("%d: %s\n", turns, av[uid + 1]);

    if (visualizer.wait())
      break;
  }

  for (int uid = 0; uid < Redcode::users; uid++)
    printf("%s: %d/%d\n", av[uid + 1], count[uid], turns);

  return 0;
}

//******************************************************************************
