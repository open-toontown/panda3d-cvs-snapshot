// Filename: namedNode.cxx
// Created by:  drose (15Jan99)
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

#include <bamWriter.h>
#include <bamReader.h>
#include <datagram.h>
#include <datagramIterator.h>
#include "namedNode.h"

TypeHandle NamedNode::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: NamedNode::make_copy
//       Access: Public, Virtual
//  Description: Returns a newly-allocated Node that is a shallow copy
//               of this one.  It will be a different Node pointer,
//               but its internal data may or may not be shared with
//               that of the original Node.
////////////////////////////////////////////////////////////////////
Node *NamedNode::
make_copy() const {
  return new NamedNode(*this);
}

////////////////////////////////////////////////////////////////////
//     Function: NamedNode::Output
//       Access: Public, Virtual
//  Description: Writes a brief description of the node to the
//               indicated output stream.  This is invoked by the <<
//               operator.  It may be overridden in derived classes to
//               include some information relevant to the class.
////////////////////////////////////////////////////////////////////
void NamedNode::
output(ostream &out) const {
  if (get_name().empty()) {
    out << get_type();
  } else {
    out << get_type() << " " << get_name();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: NamedNode::write_object
//  Description: Function to write the important information in
//               the particular object to a Datagram
////////////////////////////////////////////////////////////////////
void NamedNode::
write_datagram(BamWriter *manager, Datagram &me)
{
  Node::write_datagram(manager, me);
  me.add_string(get_name());
}

////////////////////////////////////////////////////////////////////
//     Function: NamedNode::make_NamedNode
//       Access: Protected
//  Description: Factory method to generate a node object
////////////////////////////////////////////////////////////////////
TypedWritable* NamedNode::
make_NamedNode(const FactoryParams &params)
{
  NamedNode *me = new NamedNode;
  DatagramIterator scan;
  BamReader *manager;

  parse_params(params, scan, manager);
  me->fillin(scan, manager);
  return me;
}

////////////////////////////////////////////////////////////////////
//     Function: NamedNode::fillin
//       Access: Protected
//  Description: Function that reads out of the datagram (or asks
//               manager to read) all of the data that is needed to
//               re-create this object and stores it in the appropiate
//               place
////////////////////////////////////////////////////////////////////
void NamedNode::
fillin(DatagramIterator& scan, BamReader* manager)
{
  Node::fillin(scan, manager);
  set_name(scan.get_string());
}

////////////////////////////////////////////////////////////////////
//     Function: NamedNode::register_with_factory
//       Access: Public, Static
//  Description: Factory method to generate a node object
////////////////////////////////////////////////////////////////////
void NamedNode::
register_with_read_factory(void)
{
  BamReader::get_factory()->register_factory(get_class_type(), make_NamedNode);
}
