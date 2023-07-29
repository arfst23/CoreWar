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
    int head;
    int tail;
  };

  Proc proc[Redcode::users][Redcode::procs];

  Controler &controler;

  void reset()
  {
    int i = idx[user];
    Address tail(proc[user][i].tail);
    Address head(proc[user][i].head);
    for (;;)
    {
      controler.set(tail(), user);
      if (tail.eq(head))
	return;
      tail.add(1);
    }
  }

  bool inc()
  {
    if (procs[user])
      if (++idx[user] >= procs[user])
	idx[user] = 0;

    if (users)
      do
	if (++user >= Redcode::users)
	{
	  user = 0;
	  turns++;

	  if (controler.check())
	    return true;
	}
      while (procs[user] == 0);
    return false;
  }

//******************************************************************************

public:

  Scheduler(Controler &c)
    : controler(c), delay(0)
  { }

  void init(Address *address)
  {
    users = Redcode::users;
    user = 0;
    nextpid = 0;
    turns = 0;

    for (int uid = 0; uid < Redcode::users; uid++)
    {
      procs[uid] = 1;
      idx[uid] = 0;

      proc[uid][0].adr = address[uid]();
      proc[uid][0].pid = nextpid++;
      proc[uid][0].tail = address[uid]();
      proc[uid][0].head = address[uid]();

      controler.setexec(address[uid]());
    }
  }

  int turn()
  {
    return turns;
  }

  int uids()
  {
    return users;
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
    reset();

    int i = idx[user];
    int j = procs[user] - 1;
    if (i < j)
      memmove(&proc[user][i], &proc[user][i + 1], (j - i) * sizeof(Proc));

    if (--procs[user] == 0)
      users--;
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
    proc[user][i].head = adr();
    proc[user][i].tail = adr();

    controler.setexec(adr());
    return proc[user][i].pid;
  }

  bool next(const Address &adr)
  {
    assert(users > 0);

    int i = idx[user];
    proc[user][i].adr = adr();
    Address tail(proc[user][i].tail);
    Address head(proc[user][i].head);
    if (!adr.between(tail, head))
    {
      tail.add(-1);
      head.add(1);
      if (adr.eq(tail()))
      {
	proc[user][i].tail = adr();
	controler.setexec(adr());
      }
      else if (adr.eq(head()))
      {
	proc[user][i].head = adr();
	controler.setexec(adr());
      }
      else
      {
	reset();
	proc[user][i].head = adr();
	proc[user][i].tail = adr();
	controler.setexec(adr());
      }
    }

    return inc();
  }
};

#endif

//******************************************************************************
