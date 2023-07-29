//******************************************************************************

#include "processor.h"
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

  void setexec(int address)
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
  batch.init();

  Memory<Batch> memory(batch);
  memory.init();
  Cell prog[] =
  {
    // line uid chmd dst src val
    { 1, 0, 2, 2, 0, 0 },
    { 2, 0, 0, 0, 0, 0 },
    { 3, 0, 13, 1, 0, 0 },
  };
  memory.load(0, 0, prog, 3);

  Scheduler<Batch> scheduler(batch);
  Address adr[Redcode::users] = { 0, 256 };
  scheduler.init(adr);

  Processor<Batch> processor(memory, scheduler);

  assert(scheduler.uids() == 2);
  assert(scheduler.uid() == 0);
  assert(scheduler.address()() == 0);

  processor.step();

  assert(scheduler.uids() == 2);
  assert(scheduler.uid() == 1);
  assert(scheduler.address()() == 256);

  processor.step();

  assert(scheduler.uids() == 1);
  assert(scheduler.uid() == 0);
  assert(scheduler.address()() == 2);

  processor.step();

  assert(scheduler.uids() == 1);
  assert(scheduler.uid() == 0);
  assert(scheduler.address()() == 3);

  batch.flush();
  batch.wait();
  return 0;
}

//******************************************************************************
