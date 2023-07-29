//******************************************************************************

#ifndef PROCESSOR
#define PROCESSOR
#include "memory.h"
#include "scheduler.h"
#include <stdio.h>

//******************************************************************************

template <typename Controler>
class Processor
{
  Memory<Controler> &memory;
  Scheduler<Controler> &scheduler;

public:
  Processor(Memory<Controler> &m, Scheduler<Controler> &s)
    : memory(m), scheduler(s)
  { }

  bool step()
  {
    int uid = scheduler.uid();
    int pid = scheduler.pid();
    Address adr = scheduler.address();
    int line = memory.line(adr);
    int cmd = memory.cmd(adr);

    switch (cmd)
    {
      case 0: // dat
      {
	if (Redcode::trace)
	  printf("[%d:%04d] %05d  %03d: dat -> abort\n",
	    uid, pid, adr(), line);

	return scheduler.abort();
      }

      case 1: // frk
      {
	Address dst(adr);
	dst.add(memory.dst(adr));
	int frk = scheduler.fork(dst);
	adr.add(1);

	if (frk)
	{
	  if (Redcode::trace)
	    printf("[%d:%04d] %05d  %03d: frk %d/%05d -> [%02d]\n",
	      uid, pid, adr(), line, memory.dst(adr)(), dst(), frk);
	}
	else
	  if (Redcode::trace)
	    printf("[%d:%04d] %05d  %03d: frk %d/%05d -> no more processes\n",
	      uid, pid, adr(), line, memory.dst(adr)(), dst());

	return scheduler.next(adr);
      }
      
      case 2: // jmp
      {
	Address dst(adr);
	dst.add(memory.dst(adr));

	if (Redcode::trace)
	  printf("[%d:%04d] %05d  %03d: jmp %d/%05d\n",
	    uid, pid, adr(), line, memory.dst(adr)(), dst());

	return scheduler.next(dst);
      }
      
      case 3: // jlt
      {
	Address src(adr);
	src.add(memory.src(adr));
	Number val = memory.get(src);
	if (val.cmp(memory.val(adr)) == -1)
	{
	  Address dst(adr);
	  dst.add(memory.dst(adr));

	  if (Redcode::trace)
	    printf("[%d:%04d] %05d  %03d: jlt %d/%05d %d < %d -> %d/%05d\n",
	      uid, pid, adr(), line, memory.src(adr)(), src(), val(), memory.val(adr)(),
	      memory.dst(adr)(), dst());
	  
	  return scheduler.next(dst);
	}
	
	if (Redcode::trace)
	  printf("[%d:%04d] %05d  %03d: jlt %d/%05d %d < %d -> cont\n",
	    uid, pid, adr(), line, memory.src(adr)(), src(), val(), memory.val(adr)());

	adr.add(1);
	return scheduler.next(adr);
      }

      case 4: // jle
      {
	Address src(adr);
	src.add(memory.src(adr));
	Number val = memory.get(src);
	if (val.cmp(memory.val(adr)) != 1)
	{
	  Address dst(adr);
	  dst.add(memory.dst(adr));

	  if (Redcode::trace)
	    printf("[%d:%04d] %05d  %03d: jle %d/%05d %d <= %d -> %d/%05d\n",
	      uid, pid, adr(), line, memory.src(adr)(), src(), val(), memory.val(adr)(),
	      memory.dst(adr)(), dst());
	  
	  return scheduler.next(dst);
	}
	
	if (Redcode::trace)
	  printf("[%d:%04d] %05d  %03d: jle %d/%05d %d <= %d -> cont\n",
	    uid, pid, adr(), line, memory.src(adr)(), src(), val(), memory.val(adr)());

	adr.add(1);
	return scheduler.next(adr);
      }

      case 5: // jgt
      {
	Address src(adr);
	src.add(memory.src(adr));
	Number val = memory.get(src);
	if (val.cmp(memory.val(adr)) == 1)
	{
	  Address dst(adr);
	  dst.add(memory.dst(adr));

	  if (Redcode::trace)
	    printf("[%d:%04d] %05d  %03d: jgt %d/%05d %d > %d -> %d/%05d\n",
	      uid, pid, adr(), line, memory.src(adr)(), src(), val(), memory.val(adr)(),
	      memory.dst(adr)(), dst());
	  
	  return scheduler.next(dst);
	}
	
	if (Redcode::trace)
	  printf("[%d:%04d] %05d  %03d: jgt %d/%05d %d > %d -> cont\n",
	    uid, pid, adr(), line, memory.src(adr)(), src(), val(), memory.val(adr)());

	adr.add(1);
	return scheduler.next(adr);
      }

      case 6: // jge
      {
	Address src(adr);
	src.add(memory.src(adr));
	Number val = memory.get(src);
	if (val.cmp(memory.val(adr)) != -1)
	{
	  Address dst(adr);
	  dst.add(memory.dst(adr));

	  if (Redcode::trace)
	    printf("[%d:%04d] %05d  %03d: jge %d/%05d %d >= %d -> %d/%05d\n",
	      uid, pid, adr(), line, memory.src(adr)(), src(), val(), memory.val(adr)(),
	      memory.dst(adr)(), dst());
	  
	  return scheduler.next(dst);
	}
	
	if (Redcode::trace)
	  printf("[%d:%04d] %05d  %03d: jge %d/%05d %d >= %d -> cont\n",
	    uid, pid, adr(), line, memory.src(adr)(), src(), val(), memory.val(adr)());

	adr.add(1);
	return scheduler.next(adr);
      }

      case 7: // jeq
      {
	Address src(adr);
	src.add(memory.src(adr));
	Number val = memory.get(src);
	if (val.cmp(memory.val(adr)) == 0)
	{
	  Address dst(adr);
	  dst.add(memory.dst(adr));

	  if (Redcode::trace)
	    printf("[%d:%04d] %05d  %03d: jeq %d/%05d %d == %d -> %d/%05d\n",
	      uid, pid, adr(), line, memory.src(adr)(), src(), val(), memory.val(adr)(),
	      memory.dst(adr)(), dst());
	  
	  return scheduler.next(dst);
	}
	
	if (Redcode::trace)
	  printf("[%d:%04d] %05d  %03d: jeq %d/%05d %d == %d -> cont\n",
	    uid, pid, adr(), line, memory.src(adr)(), src(), val(), memory.val(adr)());

	adr.add(1);
	return scheduler.next(adr);
      }

      case 8: // jne
      {
	Address src(adr);
	src.add(memory.src(adr));
	Number val = memory.get(src);
	if (val.cmp(memory.val(adr)) != 0)
	{
	  Address dst(adr);
	  dst.add(memory.dst(adr));

	  if (Redcode::trace)
	    printf("[%d:%04d] %05d  %03d: jne %d/%05d %d != %d -> %d/%05d\n",
	      uid, pid, adr(), line, memory.src(adr)(), src(), val(), memory.val(adr)(),
	      memory.dst(adr)(), dst());

	  return scheduler.next(dst);
	}
	
	if (Redcode::trace)
	  printf("[%d:%04d] %05d  %03d: jne %d/%05d %d != %d -> cont\n",
	    uid, pid, adr(), line, memory.src(adr)(), src(), val(), memory.val(adr)());

	adr.add(1);
	return scheduler.next(adr);
      }

      case 9: // set
      {
	Address dst(adr);
	dst.add(memory.dst(adr));
	memory.set(uid, dst, memory.val(adr));

	if (Redcode::trace)
	  printf("[%d:%04d] %05d  %03d: set %d/%05d -> %d\n",
	    uid, pid, adr(), line, memory.dst(adr)(), dst(), memory.val(adr)());

	adr.add(1);
	return scheduler.next(adr);
      }

      case 10: // inc
      {
	Address dst(adr);
	dst.add(memory.dst(adr));
	Number val = memory.get(dst);
	Number src(val);
	Number add = memory.val(adr);
	val.add(add);
	memory.set(uid, dst, val);

	if (Redcode::trace)
	  printf("[%d:%04d] %05d  %03d: inc %d/%05d %d -> %d\n",
	    uid, pid, adr(), line, memory.dst(adr)(), dst(), src(), val());

	adr.add(1);
	return scheduler.next(adr);
      }

      case 11: // dec
      {
	Address dst(adr);
	dst.add(memory.dst(adr));
	Number val = memory.get(dst);
	Number src(val);
	Number sub = memory.val(adr);
	val.sub(sub);
	memory.set(uid, dst, val);
	adr.add(1);

	if (Redcode::trace)
	  printf("[%d:%04d] %05d  %03d: dec %d/%05d %d -> %d\n",
	    uid, pid, adr(), line, memory.dst(adr)(), dst(), src(), val());

	return scheduler.next(adr);
      }

      case 12: // cpi
      {
	Address src(adr);
	src.add(memory.src(adr));
	Address dst(adr);
	dst.add(memory.dst(adr));

	Address val(adr);
	val.add(memory.val(adr));
	Number off = memory.get(val);
	dst.add(off);

	memory.copy(uid, dst, src);

	if (Redcode::trace)
	  printf("[%d:%04d] %05d  %03d: cpi %d/%05d: (%d/%05d) -> (%d + %d/%05d) %d\n",
	    uid, pid, adr(), line, memory.val(adr)(), val(),
	    memory.src(adr)(), src(), off(), memory.dst(adr)(), dst(), memory.line(src));

	adr.add(1);
	return scheduler.next(adr);
      }

      case 13: // cpy
      {
	Address src(adr);
	src.add(memory.src(adr));
	Address dst(adr);
	dst.add(memory.dst(adr));

	Address val(adr);
	val.add(memory.val(adr));
	Number off = memory.get(val);
	src.add(off);
	dst.add(off);

	memory.copy(uid, dst, src);

	if (Redcode::trace)
	  printf("[%d:%04d] %05d  %03d: cpy %d/%05d: (%d + %d/%05d) -> (%d + %d/%05d) %d\n",
	    uid, pid, adr(), line, memory.val(adr)(), val(),
	    off(), memory.src(adr)(), src(), off(), memory.dst(adr)(), dst(), memory.line(src));

	adr.add(1);
	return scheduler.next(adr);
      }
    }
    return false;
  }
};

#endif

//******************************************************************************
