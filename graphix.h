//******************************************************************************

#ifndef GRAPHIX
#define GRAPHIX

#include <vector>
#include <X11/Xlib.h>
#include <assert.h>

//******************************************************************************

class Graphix
{
private:

  int width;
  int height;
  Display *display = NULL;
  Window window;
  std::vector<GC> gc;

public:

  Graphix(int w, int h, std::vector<const char*> &colors);

  void point(int color, int x, int y)
  {
    assert(color > 0 && color < gc.size());
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    XDrawPoint(display, window, gc[color], x, y);
  }

  void line(int color, int x, int y, int xx, int yy)
  {
    assert(color > 0 && color < gc.size());
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    assert(xx >= 0 && xx < width);
    assert(yy >= 0 && yy < height);
    XDrawLine(display, window, gc[color], x, y, xx, yy);
  }

  void rect(int color, int x, int y, int w, int h)
  {
    assert(color > 0 && color < gc.size());
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    assert(x + w >= 0 && x + w < width);
    assert(y + h >= 0 && y + h < height);
    XFillRectangle(display, window, gc[color], x, y, (unsigned int)w, (unsigned int)h);
  }

  void flush()
  {
    XFlush(display);
  }

  int button()
  {
    XEvent event;
    if (XCheckMaskEvent(display, ButtonPressMask, &event))
      return reinterpret_cast<XButtonEvent*>(&event)->button;
    return 0;
  }

  int wait()
  {
    XEvent event;
    XMaskEvent(display, ButtonPressMask, &event);
    return reinterpret_cast<XButtonEvent*>(&event)->button;
  }
};

#endif

//******************************************************************************
