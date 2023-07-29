//******************************************************************************

#include "machine.h"
#include "redcode.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

//******************************************************************************

class Batch
{
public:

  Batch()
  { }

  void init()
  { }

  void set(int address, int uid)
  { }

  void setexec(int address)
  { }

  bool check()
  {
    return 0;
  }

  bool wait()
  {
    return 0;
  }
};

//******************************************************************************

int main(int ac, char *av[])
{
  assert(ac == Redcode::users + 1);

  srand(time(NULL));

  Batch batch;
  Machine<Batch> machine(batch, av + 1);

  int count[Redcode::users];
  memset(count, 0, sizeof(count));

  for (int turn = 0; turn < Redcode::turns; turn++)
  {
    int uid = machine.run();
    if (uid >= 0)
      count[uid]++;

    if (Redcode::trace)
    {
      if (uid == -1) // timeout
	printf("%03d: timeout\n", turn);
      else if (uid == -2) // terminate
	printf("%03d: all terminated\n", turn);
      else if (uid == -3) // shutdown
	printf("%03d: user shutdown\n", turn);
      else
	printf("%03d: %s\n", turn, av[uid + 1]);
    }
  }

  if (Redcode::trace)
    printf("\n");
  for (int uid = 0; uid < Redcode::users; uid++)
    printf("%s: %d\n", av[uid + 1], count[uid]);

  return 0;
}

//******************************************************************************
