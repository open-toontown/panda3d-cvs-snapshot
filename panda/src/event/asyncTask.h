// Filename: asyncTask.h
// Created by:  drose (23Aug06)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

#ifndef ASYNCTASK_H
#define ASYNCTASK_H

#include "pandabase.h"

#include "typedReferenceCount.h"
#include "namable.h"
#include "pmutex.h"
#include "conditionVar.h"

#ifdef HAVE_PYTHON

#undef HAVE_LONG_LONG  // NSPR and Python both define this.
#undef _POSIX_C_SOURCE
#include <Python.h>

#endif  // HAVE_PYTHON

class AsyncTaskManager;
class AsyncTaskChain;

////////////////////////////////////////////////////////////////////
//       Class : AsyncTask
// Description : This class represents a concrete task performed by an
//               AsyncManager.  Normally, you would subclass from this
//               class, and override do_task(), to define the
//               functionality you wish to have the task perform.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_EVENT AsyncTask : public TypedReferenceCount, public Namable {
public:
  INLINE AsyncTask(const string &name = string());
  virtual ~AsyncTask();
  ALLOC_DELETED_CHAIN(AsyncTask);

PUBLISHED:
  enum DoneStatus {
    DS_done,   // normal task completion
    DS_cont,   // run task again next epoch
    DS_again,  // run task again after get_delay() seconds
    DS_abort,  // abort the task and interrupt the whole task manager
  };

  enum State {
    S_inactive,
    S_active,
    S_servicing,
    S_servicing_removed,  // Still servicing, but wants removal from manager.
    S_sleeping,
  };

  INLINE State get_state() const;
  INLINE AsyncTaskManager *get_manager() const;

  void remove();

  INLINE void set_delay(double delay);
  INLINE void clear_delay();
  INLINE bool has_delay() const;
  INLINE double get_delay() const;
  INLINE double get_wake_time() const;
  
  INLINE double get_start_time() const;
  double get_elapsed_time() const;

  void set_name(const string &name);
  INLINE void clear_name();

  void set_task_chain(const string &chain_name);
  INLINE const string &get_task_chain() const;

  void set_sort(int sort);
  INLINE int get_sort() const;

  void set_priority(int priority);
  INLINE int get_priority() const;

  INLINE void set_done_event(const string &done_event);
  INLINE const string &get_done_event() const;

#ifdef HAVE_PYTHON
  INLINE void set_python_object(PyObject *python_object);
  INLINE PyObject *get_python_object() const;
#endif  // HAVE_PYTHON

  virtual void output(ostream &out) const;

protected:
  virtual DoneStatus do_task();

protected:
  string _chain_name;
  double _delay;
  bool _has_delay;
  double _wake_time;
  double _start_time;
  int _sort;
  int _priority;
  string _done_event;
  State _state;
  Thread *_servicing_thread;
  AsyncTaskManager *_manager;
  AsyncTaskChain *_chain;

private:
#ifdef HAVE_PYTHON
  PyObject *_python_object;
#endif  // HAVE_PYTHON
  
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "AsyncTask",
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

  friend class AsyncTaskManager;
  friend class AsyncTaskChain;
};

INLINE ostream &operator << (ostream &out, const AsyncTask &task) {
  task.output(out);
  return out;
};

#include "asyncTask.I"

#endif
