//******************************************************************************

#include "graphix.h"

#include <stdlib.h>
#include <X11/Xatom.h>

//******************************************************************************
// https://tronche.com/gui/x/xlib/events/processing-overview.html

Graphix::Graphix(int w, int h, std::vector<const char*> &colors)
{
  width = w;
  height = h;
  assert(width > 0);
  assert(height > 0);

  display = XOpenDisplay(NULL);
  assert(display);
  XSync(display, False);
  int screen = DefaultScreen(display);
  window = XCreateSimpleWindow
  (
    display, RootWindow(display, screen),
    0, 0, // x, y
    (unsigned int)width, (unsigned int)height, // w, h
    1, // border
    BlackPixel(display, screen), BlackPixel(display, screen)
  );
  XSelectInput(display, window, ExposureMask | StructureNotifyMask);

  {
    typedef struct Hints
    {
      unsigned long   flags;
      unsigned long   functions;
      unsigned long   decorations;
      long            inputMode;
      unsigned long   status;
    } Hints;

    Hints hints;
    hints.flags = 2;
    hints.decorations = 0;
    Atom property = XInternAtom(display, "_MOTIF_WM_HINTS", true);
    XChangeProperty(display, window, property, property, 32, PropModeReplace,
      (unsigned char*)&hints,5);
  }

  XMapWindow(display, window);
  XFlush(display);
  for (int events = 0; events < 4;)
  {
    XEvent event;
    XNextEvent(display, &event);
    if (event.type == Expose)
      events++;
    if (event.type == MapNotify)
      events++;
  }
  XSelectInput(display, window, ButtonPressMask);

  assert(!colors.empty());
  gc.resize(colors.size());

  Colormap colormap = DefaultColormap(display, screen);
  for (int color = 0; color < colors.size(); color++)
  {
    XColor xcolor;
    Status status = XAllocNamedColor(display, colormap, colors[color], &xcolor, &xcolor);
    assert(status);

    XGCValues values;
    gc[color] = XCreateGC(display, window, 0L, &values);
    assert(gc[color]);

    XSetForeground(display, gc[color], xcolor.pixel);
    XSetBackground(display, gc[color],
      BlackPixel(display, screen));
    XSetLineAttributes(display, gc[color], 1L, LineSolid, CapButt, JoinBevel);
    XSetFillStyle(display, gc[color], FillSolid);
  }
}

//******************************************************************************
