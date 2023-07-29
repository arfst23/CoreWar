//******************************************************************************

#include "loader.h"
#include <assert.h>

//******************************************************************************

int main(int ac, char *av[])
{
  assert(ac == 2);

  Loader loader;
  loader.load(av[1]);

  assert(loader.length() == 6);

  const Cell *prog = loader.program();

  assert(prog[0].line == 8);
  assert(prog[0].cmd == 9);
  assert(prog[0].dst() == 5);
  assert(prog[0].src() == 0);
  assert(prog[0].val() == 0);

  assert(prog[1].line == 10);
  assert(prog[1].cmd == 13);
  assert(prog[1].dst() == 255);
  assert(prog[1].src() == -1);
  assert(prog[1].val() == 4);

  assert(prog[2].line == 11);
  assert(prog[2].cmd == 10);
  assert(prog[2].dst() == 3);
  assert(prog[2].src() == 0);
  assert(prog[2].val() == 1);

  assert(prog[3].line == 12);
  assert(prog[3].cmd == 4);
  assert(prog[3].dst() == -2);
  assert(prog[3].src() == 2);
  assert(prog[3].val() == 5);

  assert(prog[4].line == 13);
  assert(prog[4].cmd == 2);
  assert(prog[4].dst() == 252);
  assert(prog[4].src() == 0);
  assert(prog[4].val() == 0);

  assert(prog[5].line == 17);
  assert(prog[5].cmd == 0);
  assert(prog[5].dst() == 0);
  assert(prog[5].src() == 0);
  assert(prog[5].val() == 0);

  return 0;
}

//******************************************************************************
