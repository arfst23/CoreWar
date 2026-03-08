//******************************************************************************

#include "visualizer.h"
#include "graphix.h"
#include <unistd.h>

//******************************************************************************

enum
{
  hscale = 4,
  vscale = 4,
  offset = 6,

  maxdelay = 32768,
  mindelay = 1,
  defdelay = 1024,
};

enum Color
{
  black,
  grey,
  red,
  green,
  blue,
  magenta,
  yellow,
  cyan,
};

Visualizer::Visualizer()
  : delay(defdelay)
{
  int width = 2 * offset + hscale * (2 * Redcode::width / 2 - 1);
  int height = 2 * offset + vscale * (2 * Redcode::height / 2 - 1);

  std::vector<const char*> colors =
  {
    "black",
    "grey25",
    "#bb0000",
    "#00a100",
    "#0000f0",
    "#ff9020",
    "#20ff20",
    "#20c0ff",
  };

  graphix = new Graphix(width, height, colors);
}

Visualizer::~Visualizer()
{
  delete graphix;
}

void Visualizer::init()
{
  for (int row = 0; row < Redcode::height / 2; row++)
    for (int col = 0; col < Redcode::width / 2; col++)
    {
      int x = offset + 2 * hscale * col;
      int y = offset + 2 * vscale * row;
      graphix->rect(grey, x, y, hscale, vscale);
    }
  graphix->flush();
}

void Visualizer::set(int address, int uid)
{
  assert(address >= 0 && address < Redcode::size);
  assert(uid >= -1 && uid < Redcode::users);
  int row = address / (Redcode::width * 2);
  int col = address % (Redcode::width * 2);
  col /= 4;
  int x = offset + 2 * hscale * col;
  int y = offset + 2 * vscale * row;
  graphix->rect(red + uid, x, y, 2 * hscale, 2 * vscale);
}

void Visualizer::setexec(int address, int uid)
{
  assert(address >= 0 && address < Redcode::size);
  int row = address / (Redcode::width * 2);
  int col = address % (Redcode::width * 2);
  col /= 4;
  int x = offset + 2 * hscale * col;
  int y = offset + 2 * vscale * row;
  graphix->rect(magenta + uid, x, y, 2 * hscale, 2 * vscale);
}

bool Visualizer::check()
{
  graphix->flush();
  while (int button = graphix->button())
  {
    switch (button)
    {
    case 1: // left
      if (wait())
	return true;
      break;

    case 3: // right
      return true;

    case 4: // up
      if (delay > mindelay)
	delay >>= 1;
      break;
      
    case 5: // down
      if (delay < maxdelay)
	delay <<= 1;
      break;

    case 8: // back
      delay = maxdelay;
      break;

    case 9: // forward
      delay = defdelay;
      break;
    }
  }
  if (delay)
  {
    usleep(delay);
    usleep(delay);
    usleep(delay);
    usleep(delay);
    usleep(delay);
  }
  return false;
}

bool Visualizer::wait()
{
  graphix->flush();
  for (;;)
  {
    int button = graphix->wait();
    if (button == 1) // left
      return false;
    if (button == 3) // right
      return true;
  }
}

//******************************************************************************
