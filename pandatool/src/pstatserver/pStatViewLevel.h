// Filename: pStatViewLevel.h
// Created by:  drose (11Jul00)
// 
////////////////////////////////////////////////////////////////////

#ifndef PSTATVIEWLEVEL_H
#define PSTATVIEWLEVEL_H

#include <pandatoolbase.h>

#include <vector>

class PStatClientData;

////////////////////////////////////////////////////////////////////
//       Class : PStatViewLevel
// Description : This is a single level value, or band of color,
//               within a View.  
//
//               It generally indicates either the elapsed time, or
//               the "level" value, for a particular Collector within
//               a given frame for a particular thread.
////////////////////////////////////////////////////////////////////
class PStatViewLevel {
public:
  INLINE int get_collector() const;
  INLINE float get_value_alone() const;
  float get_net_value() const;

  void sort_children(const PStatClientData *client_data);

  int get_num_children() const;
  const PStatViewLevel *get_child(int n) const;

private:
  int _collector;
  float _value_alone;
  PStatViewLevel *_parent;

  typedef vector<PStatViewLevel *> Children;
  Children _children;

  friend class PStatView;
};

#include "pStatViewLevel.I"

#endif
