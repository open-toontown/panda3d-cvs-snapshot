// Filename: winStatsMonitor.h
// Created by:  drose (02Dec03)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://www.panda3d.org/license.txt .
//
// To contact the maintainers of this program write to
// panda3d@yahoogroups.com .
//
////////////////////////////////////////////////////////////////////

#ifndef WINSTATSMONITOR_H
#define WINSTATSMONITOR_H

#include "pandatoolbase.h"

#include "winStatsGraph.h"
#include "pStatMonitor.h"
#include "pointerTo.h"
#include "pset.h"

#include <windows.h>

////////////////////////////////////////////////////////////////////
//       Class : WinStatsMonitor
// Description : This class represents a connection to a PStatsClient
//               and manages the data exchange with the client.
////////////////////////////////////////////////////////////////////
class WinStatsMonitor : public PStatMonitor {
public:
  WinStatsMonitor();
  virtual ~WinStatsMonitor();

  virtual string get_monitor_name();

  virtual void initialized();
  virtual void got_hello();
  virtual void got_bad_version(int client_major, int client_minor,
                               int server_major, int server_minor);
  virtual void new_collector(int collector_index);
  virtual void new_data(int thread_index, int frame_number);
  virtual void lost_connection();
  virtual void idle();
  virtual bool has_idle();

  HWND get_window() const;
  
private:
  void add_graph(WinStatsGraph *graph);
  void remove_graph(WinStatsGraph *graph);

  void create_window();
  static void register_window_class(HINSTANCE application);

  static LONG WINAPI static_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
  LONG WINAPI window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

  typedef pset<WinStatsGraph *> Graphs;
  Graphs _graphs;

  HWND _window;
  string _window_title;

  static bool _window_class_registered;
  static const char * const _window_class_name;

  friend class WinStatsGraph;
};

#endif
