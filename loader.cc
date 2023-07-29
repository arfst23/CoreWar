//******************************************************************************

#include "loader.h"
#include "redcode.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

//******************************************************************************

void Loader::load(const char *name)
{
  int f = open(name, O_RDONLY);
  assert(f >= 0);

  struct stat s;
  fstat(f, &s);
  int size = s.st_size;
  assert(size % 26 == 0);
  len = size / 26;
  assert(len <= Redcode::length);

  for (int i = 0; i < len; i++)
  {
    char str[26];
    int r = read(f, str, 26);
    assert(r == 26);

    char *end;
    memory[i].line = strtol(str, &end, 16);
    assert(end == str + 3);

    assert(str[3] == ':');
    assert(str[4] == ' ');

    memory[i].uid = 0;

    memory[i].cmd = strtol(str + 5, &end, 16);
    assert(memory[i].cmd <= 13);
    assert(end == str + 7);

    assert(str[7] == ' ');

    int dst = strtol(str + 9, &end, 16);
    assert(dst < Redcode::range);
    assert(end == str + 13);
    memory[i].dst = dst;
    if (str[8] == '-')
      memory[i].dst = -dst;
    else
      assert(str[8] == '+');

    assert(str[13] == ' ');

    int src = strtol(str + 15, &end, 16);
    assert(src < Redcode::range);
    assert(end == str + 19);
    memory[i].src = src;
    if (str[14] == '-')
      memory[i].src = -src;
    else
      assert(str[14] == '+');

    assert(str[19] == ' ');

    int val = strtol(str + 21, &end, 16);
    assert(val < Redcode::range);
    assert(end == str + 25);
    memory[i].val = val;
    if (str[20] == '-')
      memory[i].val = -val;
    else
      assert(str[20] == '+');

    assert(str[25] == '\n');
  }
  close(f);
}

//******************************************************************************
