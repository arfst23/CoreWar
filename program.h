//******************************************************************************

#ifndef PROGRAM
#define PROGRAM

//******************************************************************************

class Program
{
  int size;
  int8_t command[Redcode::size];
  int8_t user[Redcode::size];
  int16_t lineno[Redcode::size];
  Address source[Redcode::size];
  Address destination[Redcode::size];
  Number value[Redcode::size];

public:

  Program(const char *file);

  int size()
  int cmd(Address adr)
  int line(Address adr)
  Address dst(Address adr)
  Address src(Address adr)
  Number val(Address adr)
}

#endif

//******************************************************************************
