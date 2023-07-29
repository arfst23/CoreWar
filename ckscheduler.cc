//******************************************************************************

#include "scheduler.h"
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

int main()
{
  Batch batch;
  batch.init();

  Scheduler<Batch> scheduler(batch);
  Address adr[Redcode::users] = { 0, 256 };
  scheduler.init(adr);

  // 0 0 0->1
  assert(scheduler.turn() == 0);
  assert(scheduler.uids() == 2);
  assert(scheduler.uid() == 0);
  assert(scheduler.pid() == 0);
  assert(scheduler.address()() == 0);
  assert(scheduler.fork(10) == 2);
  scheduler.next(1);

  // 1 1 256->257
  assert(scheduler.turn() == 0);
  assert(scheduler.uids() == 2);
  assert(scheduler.uid() == 1);
  assert(scheduler.pid() == 1);
  assert(scheduler.address()() == 256);
  assert(scheduler.fork(266) == 3);
  assert(!scheduler.next(257));

  // 0 2 10->X
  assert(scheduler.turn() == 1);
  assert(scheduler.uids() == 2);
  assert(scheduler.uid() == 0);
  assert(scheduler.pid() == 2);
  assert(scheduler.address()() == 10);
  scheduler.abort();

  // 1 3 266->267
  assert(scheduler.turn() == 1);
  assert(scheduler.uids() == 2);
  assert(scheduler.uid() == 1);
  assert(scheduler.pid() == 3);
  assert(scheduler.address()() == 266);
  assert(!scheduler.next(267));

  // 0 0 1->20
  assert(scheduler.turn() == 2);
  assert(scheduler.uids() == 2);
  assert(scheduler.uid() == 0);
  assert(scheduler.pid() == 0);
  assert(scheduler.address()() == 1);
  assert(!scheduler.next(20));

  // 1 1 257->X
  assert(scheduler.turn() == 2);
  assert(scheduler.uids() == 2);
  assert(scheduler.uid() == 1);
  assert(scheduler.pid() == 1);
  assert(scheduler.address()() == 257);
  assert(!scheduler.abort());
  
  // 0 0 20->19
  assert(scheduler.turn() == 3);
  assert(scheduler.uids() == 2);
  assert(scheduler.uid() == 0);
  assert(scheduler.pid() == 0);
  assert(scheduler.address()() == 20);
  assert(!scheduler.next(19));

  // 1 3 267->X
  assert(scheduler.turn() == 3);
  assert(scheduler.uids() == 2);
  assert(scheduler.uid() == 1);
  assert(scheduler.pid() == 3);
  assert(scheduler.address()() == 267);
  assert(!scheduler.abort());

  assert(scheduler.turn() == 4);
  assert(scheduler.uids() == 1);
  assert(scheduler.uid() == 0);
  assert(scheduler.pid() == 0);
  assert(scheduler.address()() == 19);

  batch.wait();
  return 0;
}

//******************************************************************************
