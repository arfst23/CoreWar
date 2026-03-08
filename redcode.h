//******************************************************************************

#ifndef REDCODE
#define REDCODE

namespace Redcode
{
  enum
  {
    width = 256,
    height = 256,
    size = 256 * 256, // memory size
    range = 256 * (32 + 1), // number range
    length = 256, // max program length

    timeout = 256 * 256 * 12,
    turns = 100,
    
    trace = 0,
    users = 3,
    procs = 4,
  };
}

#endif

//******************************************************************************
