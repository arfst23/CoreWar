//******************************************************************************

#include "memory.h"
#include <assert.h>

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

  void flush()
  { }

  bool check()
  {
    return false;
  }

  bool wait()
  {
    return false;
  }
};

//******************************************************************************

int main()
{
  Batch batch;
  Cell prog[2] =
  {
    { 23, 0, 3, 4, 5, 6 },
    { 24, 0, 0, 0, 0, 42 },
  };

  Memory<Batch> memory(batch);
  memory.init();
  memory.load(1, 0, prog, 2);
  assert(memory.line(0) == 23);
  assert(memory.uid(0) == 1);
  assert(memory.cmd(0) == 3);
  assert(memory.dst(0)() == 4);
  assert(memory.src(0)() == 5);
  assert(memory.val(0)() == 6);
  assert(memory.get(0)() == 0);

  assert(memory.line(1) == 24);
  assert(memory.uid(1) == 1);
  assert(memory.cmd(1) == 0);
  assert(memory.dst(1)() == 0);
  assert(memory.src(1)() == 0);
  assert(memory.val(1)() == 42);
  assert(memory.get(1)() == 42);

  memory.copy(1, 2, 0);
  assert(memory.line(2) == 23);
  assert(memory.uid(2) == 1);
  assert(memory.cmd(2) == 3);
  assert(memory.dst(2)() == 4);
  assert(memory.src(2)() == 5);
  assert(memory.val(2)() == 6);
  assert(memory.get(2)() == 0);

  memory.copy(1, 3, 0);
  memory.set(0, 3, 7);
  assert(memory.line(3) == 0);
  assert(memory.uid(3) == 0);
  assert(memory.cmd(3) == 0);
  assert(memory.dst(3)() == 0);
  assert(memory.src(3)() == 0);
  assert(memory.val(3)() == 7);
  assert(memory.get(3)() == 7);

  assert(memory.line(4) == 0);
  assert(memory.uid(4) == -1);
  assert(memory.cmd(4) == 0);
  assert(memory.dst(4)() == 0);
  assert(memory.src(4)() == 0);
  assert(memory.val(4)() == 0);
  assert(memory.get(4)() == 0);

  batch.flush();
  batch.wait();

  return 0;
}

//******************************************************************************
