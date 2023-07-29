//******************************************************************************

#ifndef MACHINE
#define MACHINE

#include "scheduler.h"
#include "processor.h"
#include "loader.h"
#include "memory.h"
#include "address.h"

//******************************************************************************

template <typename Controler>
class Machine
{
public:

  enum
  {
    timeout = -1,
    terminate = -2,
    shutdown = -3,
  };

private:

  Controler &controler;
  Memory<Controler> memory;
  Scheduler<Controler> scheduler;
  Processor<Controler> processor;

  Loader loader[Redcode::users];

  void init()
  {
    controler.init();
    memory.init();

    Address adr[Redcode::users];
    if (Redcode::users == 1)
      adr[0] = Redcode::height / 3 * Redcode::width + Redcode::width / 3;
    else
    {
      adr[0] = rand() % Redcode::size;
      adr[1] = adr[0]() + Redcode::range + rand() % (Redcode::size - 2 * Redcode::range);
      if (Redcode::users > 2)
      {
	int dist0, dist1;
	if (adr[0]() < adr[1]())
	{
	  dist0 = adr[1]() - adr[0]();
	  dist1 = Redcode::size - dist0;
	}
	else // adr[1]() < adr[0]()
	{
	  dist1 = adr[0]() - adr[1]();
	  dist0 = Redcode::size - dist1;
	}
	if (dist0 >= dist1)
	  adr[2] = adr[0]() + rand() % (dist0 - 2 * Redcode::range);
	else
	  adr[2] = adr[1]() + rand() % (dist1 - 2 * Redcode::range);
      }
    }

    for (int uid = 0; uid < Redcode::users; uid++)
      memory.load(uid, adr[uid], loader[uid].program(), loader[uid].length());

    scheduler.init(adr);
    controler.check();
  }

public:

  Machine(Controler &ctl, char *const *name)
    : controler(ctl), memory(ctl), scheduler(ctl), processor(memory, scheduler)
  {
    for (int uid = 0; uid < Redcode::users; uid++)
      loader[uid].load(name[uid]);
  }

  int run()
  {
    init();

    for (;;)
    {
      if (processor.step())
	return shutdown;

      int uids = scheduler.uids();
      if (Redcode::users > 1 && uids == 1)
	return scheduler.uid();
      if (uids == 0)
	return terminate;

      if (scheduler.turn() > Redcode::timeout)
	return timeout;
    }
  }
};

#endif

//******************************************************************************
