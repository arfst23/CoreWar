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

  void setexec(int address, int uid)
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
    // line uid cmd dst src val
    { 1, 0, 2, 2, 0, 0 }, // frk 2
    { 2, 0, 0, 0, 0, 0 }, // dat
    { 3, 0, 13, 1, 0, 0 }, // cpi
  };
  memory.load(0, 0, prog, 3);

  Scheduler<Batch> scheduler(batch);
  Address adr[2] = { 0, 256 };
  scheduler.init(adr, 2);

  Processor<Batch> processor(memory, scheduler);

  assert(scheduler.uids() == 2);
  assert(scheduler.uid() == 0);
  assert(scheduler.pid() == 0);
  assert(scheduler.address()() == 0);
  processor.step(); // u=0 frk 2

  assert(scheduler.uids() == 2);
  assert(scheduler.uid() == 1);
  assert(scheduler.pid() == 1);
  assert(scheduler.address()() == 256);
  processor.step(); // u=1 dat

  assert(scheduler.uids() == 1);
  assert(scheduler.uid() == 0);
  assert(scheduler.pid() == 2);
  assert(scheduler.address()() == 2);
  processor.step(); // u=1 cpi

  assert(scheduler.uids() == 1);
  assert(scheduler.uid() == 0);
  assert(scheduler.pid() == 0);
  assert(scheduler.address()() == 1);
  processor.step(); // u=1  dat

  assert(scheduler.uids() == 1);
  assert(scheduler.uid() == 0);
  assert(scheduler.pid() == 2);
  assert(scheduler.address()() == 3);

  batch.flush();
  batch.wait();
  return 0;
}

//******************************************************************************
