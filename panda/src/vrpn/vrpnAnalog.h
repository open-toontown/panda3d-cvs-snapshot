// Filename: vrpnAnalog.h
// Created by:  drose (26Jan01)
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

#ifndef VRPNANALOG_H
#define VRPNANALOG_H

#include <pandabase.h>

#include "vrpn_interface.h"

#include "pvector.h"

class VrpnAnalogDevice;

////////////////////////////////////////////////////////////////////
//       Class : VrpnAnalog
// Description : This is the actual interface to a particular VRPN
//               analog device, and all of its numbered controls.  A
//               pointer to this object is stored in the VrpnClient
//               class for each differently-named VRPN analog device
//               we connect to.
//
//               The VRPN callbacks go here, which in turn get
//               vectored out to any VrpnAnalogDevice objects that
//               register with this.  When the last VrpnAnalogDevice
//               object unregisters, the VrpnAnalog will be deleted
//               by the VrpnClient.
//
//               This class does not need to be exported from the DLL.
////////////////////////////////////////////////////////////////////
class VrpnAnalog {
public:
  VrpnAnalog(const string &analog_name, vrpn_Connection *connection);
  ~VrpnAnalog();

  INLINE const string &get_analog_name() const;
  INLINE bool is_empty() const;

  void mark(VrpnAnalogDevice *device);
  void unmark(VrpnAnalogDevice *device);

  INLINE void poll();

  void output(ostream &out) const;
  void write(ostream &out, int indent_level = 0) const;

private:
  static void
  vrpn_analog_callback(void *userdata, const vrpn_ANALOGCB info);

private:
  string _analog_name;
  vrpn_Analog_Remote *_analog;

  typedef pvector<VrpnAnalogDevice *> Devices;
  Devices _devices;
};

INLINE ostream &operator << (ostream &out, const VrpnAnalog &analog) {
  analog.output(out);
  return out;
}

#include "vrpnAnalog.I"

#endif
