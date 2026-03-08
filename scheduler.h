//******************************************************************************

#ifndef SCHEDULER
#define SCHEDULER

#include "address.h"
#include "redcode.h"
#include <string.h>
#include <unistd.h>
#include <assert.h>

//******************************************************************************

template <typename Controler>
class Scheduler
{
  int users;
  int running;
  int user;
  int nextpid;
  int procs[Redcode::users];
  int idx[Redcode::users];
  int turns;
  int delay;

  struct Proc
  {
    int adr;
    int pid;
  };

  Proc proc[Redcode::users][Redcode::procs];

  Controler &controler;

  bool inc()
  {
    if (controler.check())
      return true;

    if (procs[user])
      if (++idx[user] >= procs[user])
	idx[user] = 0;

    if (users)
      do
	if (++user >= users)
	{
	  user = 0;
	  turns++;
	}
      while (procs[user] == 0);
    return false;
  }

//******************************************************************************

public:

  Scheduler(Controler &c)
    : controler(c), delay(0)
  { }

  void init(Address *address, int u)
  {
    users = u;
    running = u;
    user = 0;
    nextpid = 0;
    turns = 0;

    for (int uid = 0; uid < u; uid++)
    {
      procs[uid] = 1;
      idx[uid] = 0;

      proc[uid][0].adr = address[uid]();
      proc[uid][0].pid = nextpid++;

      controler.setexec(address[uid](), uid);
    }
  }

  int turn()
  {
    return turns;
  }

  int uids()
  {
    return running;
  }

  int uid() const
  {
    return user;
  }

  int pid() const
  {
    int i = idx[user];
    return proc[user][i].pid;
  }
  
  Address address()
  {
    int i = idx[user];
    return proc[user][i].adr;
  }

//******************************************************************************

  bool abort()
  {
    int i = idx[user];
    int j = procs[user] - 1;
    if (i < j)
      memmove(&proc[user][i], &proc[user][i + 1], (j - i) * sizeof(Proc));

    if (--procs[user] == 0)
      running--;
    else
      idx[user]--;

    return inc();
  }

  int fork(const Address &adr)
  {
    if (procs[user] == Redcode::procs)
      return -1;

    int i = procs[user]++;
    proc[user][i].adr = adr();
    proc[user][i].pid = nextpid++;

    controler.setexec(adr(), user);
    return proc[user][i].pid;
  }

  bool next(const Address &adr)
  {
    assert(running > 0);

    int i = idx[user];
    proc[user][i].adr = adr();
    controler.setexec(adr(), user);

    return inc();
  }
};

#endif

//******************************************************************************
