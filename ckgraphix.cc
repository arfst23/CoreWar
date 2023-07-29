//******************************************************************************

#include "graphix.h"
#include <iostream>

//******************************************************************************

enum Color
{
  black,
  grey,
  yellow,
  red,
  green,
  blue,
};

int main()
{
  std::vector<const char*> colors =
  {
    "black",
    "grey40",
    "yellow",
    "red2",
    "green3",
    "blue2",
  };

  Graphix graphix(800, 400, colors);
  
  graphix.rect(grey, 50, 50, 100, 300);
  graphix.rect(yellow, 200, 50, 100, 300);
  graphix.rect(red, 350, 50, 100, 300);
  graphix.rect(green, 500, 50, 100, 300);
  graphix.rect(blue, 650, 50, 100, 300);
  graphix.flush();

  int button = graphix.wait();
  std::cout << "button=" << button << std::endl;
  return 0;
}

//******************************************************************************
