// Filename: glxDisplay.cxx
// Created by:  drose (30Oct00)
// 
////////////////////////////////////////////////////////////////////

#include "glxDisplay.h"
#include "glxGraphicsWindow.h"
#include "config_glxdisplay.h"

#include <graphicsPipe.h>

#include <GL/glx.h>

TypeHandle glxDisplay::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: glxDisplay::Constructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
glxDisplay::
glxDisplay(GraphicsPipe *pipe, const string &x_specifier) {
  _pipe = pipe;
  _display = XOpenDisplay(x_specifier.c_str());
  if (!_display) {
    glxdisplay_cat.fatal()
      << "glxGraphicsPipe::construct(): Could not open display: "
      << x_specifier << endl;
    exit(1);
  }
  int errorBase, eventBase;
  if (!glXQueryExtension(_display, &errorBase, &eventBase)) {
    glxdisplay_cat.fatal()
      << "glxGraphicsPipe::construct(): OpenGL GLX extension not "
      << "supported by display: " << x_specifier << endl;
    exit(1);
  }
  _screen = DefaultScreen(_display);
  _root = RootWindow(_display, _screen);
  _width = DisplayWidth(_display, _screen);
  _height = DisplayHeight(_display, _screen);
}


////////////////////////////////////////////////////////////////////
//     Function: glxDisplay::find_window
//       Access: Public
//  Description: Find the window that has the xwindow "win" in the
//               window list for the pipe (if it exists)
////////////////////////////////////////////////////////////////////
glxGraphicsWindow *glxDisplay::
find_window(Window win) const {
  int num_windows = _pipe->get_num_windows();
  for (int w = 0; w < num_windows; w++) {
    glxGraphicsWindow *window;
    DCAST_INTO_R(window, _pipe->get_window(w), NULL);
    if (window->get_xwindow() == win) {
      return window;
    }
  }
  return NULL;
}
