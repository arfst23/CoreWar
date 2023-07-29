//******************************************************************************

#ifndef MEMORY
#define MEMORY
#include "address.h"
#include <cstring>
#include <assert.h>

//******************************************************************************

struct Cell
{
  short line;
  char uid;
  char cmd;
  Number dst;
  Number src;
  Number val;
};

template <typename Controler>
class Memory
{
private:

  Cell memory[Redcode::size];
  Controler &controler;

public:

  Memory(Controler &c)
    : controler(c)
  { }

  void init()
  {
    memset(memory, 0, sizeof(memory));
    for (int i = 0; i < Redcode::size; i++)
      memory[i].uid = -1;
    controler.init();
  }

  void load(int uid, Address adr, const Cell *prog, int len)
  {
    assert(uid >= 0 && uid < Redcode::users);
    assert(len > 0 && len < Redcode::length);
    for (int i = 0; i < len; i++)
    {
      memory[adr()] = prog[i];
      memory[adr()].uid = uid;
      controler.set(adr(), uid);
      adr.add(1);
    }
  }

  void copy(int uid, Address dst, Address src)
  {
    assert(uid >= 0 && uid < Redcode::users);
    if (memory[dst()].uid != uid)
      controler.set(dst(), uid);
    memory[dst()] = memory[src()];
    memory[dst()].uid = uid;
  }

  void set(int uid, Address dst, Number val)
  {
    assert(uid >= 0 && uid < Redcode::users);
    if (memory[dst()].uid != uid)
      controler.set(dst(), uid);
    memset(memory + dst(), 0, sizeof(Cell));
    memory[dst()].val = val;
    memory[dst()].uid = uid;
  }

//******************************************************************************

  int line(Address adr)
  {
    return memory[adr()].line;
  }

  int uid(Address adr)
  {
    return memory[adr()].uid;
  }

  int cmd(Address adr)
  {
    return memory[adr()].cmd;
  }

  Number dst(Address adr)
  {
    return memory[adr()].dst;
  }

  Number src(Address adr)
  {
    return memory[adr()].src;
  }

  Number val(Address adr)
  {
    return memory[adr()].val;
  }

  Number get(Address adr)
  {
    return memory[adr()].cmd ? 0 : memory[adr()].val;
  }
};

#endif

//******************************************************************************
