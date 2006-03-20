// Filename: wdxGraphicsWindow9.h
// Created by:  mike (09Jan97)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//
////////////////////////////////////////////////////////////////////

#ifndef WDXGRAPHICSWINDOW9_H
#define WDXGRAPHICSWINDOW9_H

#include "pandabase.h"
#include "winGraphicsWindow.h"
#include "dxGraphicsStateGuardian9.h"
#include "dxInput9.h"
#include "wdxGraphicsPipe9.h"

class wdxGraphicsPipe9;

////////////////////////////////////////////////////////////////////
//       Class : wdxGraphicsWindow9
// Description : A single graphics window for rendering DirectX under
//               Microsoft Windows.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDADX wdxGraphicsWindow9 : public WinGraphicsWindow {
public:
  wdxGraphicsWindow9(GraphicsPipe *pipe,
                     const string &name,
                     int x_size, int y_size, int flags,
                     GraphicsStateGuardian *gsg,
                     GraphicsOutput *host);
  virtual ~wdxGraphicsWindow9();

  virtual bool begin_frame(FrameMode mode);
  virtual void end_frame(FrameMode mode);
  virtual void end_flip();

  virtual int verify_window_sizes(int numsizes, int *dimen);

protected:
  virtual void close_window();
  virtual bool open_window();
  virtual void reset_window(bool swapchain);

  virtual void fullscreen_restored(WindowProperties &properties);
  virtual void handle_reshape();
  virtual bool do_fullscreen_resize(int x_size, int y_size);

private:
  struct DXDeviceInfo {
    UINT cardID;
    char szDriver[MAX_DEVICE_IDENTIFIER_STRING];
    char szDescription[MAX_DEVICE_IDENTIFIER_STRING];
    GUID guidDeviceIdentifier;
    DWORD VendorID, DeviceID;
    HMONITOR _monitor;
  };
  typedef pvector<DXDeviceInfo> DXDeviceInfoVec;

  bool create_screen_buffers_and_device(DXScreenData &display,
                                        bool force_16bpp_zbuffer);

  bool choose_device();
  bool search_for_device(wdxGraphicsPipe9 *dxpipe, DXDeviceInfo *device_info);

  bool reset_device_resize_window(UINT new_xsize, UINT new_ysize);
  void init_resized_window();
  void make_current();
  static int D3DFMT_to_DepthBits(D3DFORMAT fmt);
  static bool is_badvidmem_card(D3DADAPTER_IDENTIFIER9 *pDevID);

  DXGraphicsStateGuardian9 *_dxgsg;
  DXScreenData _wcontext;

  int _buffer_mask;
  int _depth_buffer_bpp;
  bool _awaiting_restore;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    WinGraphicsWindow::init_type();
    register_type(_type_handle, "wdxGraphicsWindow9",
                  WinGraphicsWindow::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
  friend class wdxGraphicsPipe9;
};


#include "wdxGraphicsWindow9.I"

#endif
