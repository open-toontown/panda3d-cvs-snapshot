// Filename: reMutexDirect.h
// Created by:  drose (13Feb06)
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

#ifndef REMUTEXDIRECT_H
#define REMUTEXDIRECT_H

#include "pandabase.h"
#include "mutexImpl.h"
#include "conditionVarImpl.h"

class Thread;

////////////////////////////////////////////////////////////////////
//       Class : ReMutexDirect
// Description : This class implements a standard reMutex by making
//               direct calls to the underlying implementation layer.
//               It doesn't perform any debugging operations.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAEXPRESS ReMutexDirect {
protected:
  INLINE ReMutexDirect();
  INLINE ~ReMutexDirect();
private:
  INLINE ReMutexDirect(const ReMutexDirect &copy);
  INLINE void operator = (const ReMutexDirect &copy);

public:
#ifdef HAVE_REMUTEXIMPL
  INLINE void lock() const;
  INLINE void release() const;
#else
  void lock() const;
  void release() const;
#endif

  INLINE bool debug_is_locked() const;

  void output(ostream &out) const;

private:
#ifdef HAVE_REMUTEXIMPL
  ReMutexImpl _impl;

#else
  // If we don't have a reentrant mutex, we have to hand-roll one.
  Thread *_locking_thread;
  int _lock_count;
  ConditionVarImpl _cvar;

  static MutexImpl _global_lock;
#endif  // HAVE_REMUTEXIMPL
};

INLINE ostream &
operator << (ostream &out, const ReMutexDirect &m) {
  m.output(out);
  return out;
}

#include "reMutexDirect.I"

#endif
