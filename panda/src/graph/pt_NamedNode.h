// Filename: pt_NamedNode.h
// Created by:  drose (16May00)
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

#ifndef PT_NAMEDNODE_H
#define PT_NAMEDNODE_H

#include <pandabase.h>

#include "namedNode.h"

#include <pointerTo.h>

////////////////////////////////////////////////////////////////////
//       Class : PT_NamedNode
// Description : A PT(NamedNode).  This is defined here solely we can
//               explicitly export the template class.  It's not
//               strictly necessary, but it doesn't hurt.
////////////////////////////////////////////////////////////////////

EXPORT_TEMPLATE_CLASS(EXPCL_PANDA, EXPTP_PANDA, PointerToBase<NamedNode>)
EXPORT_TEMPLATE_CLASS(EXPCL_PANDA, EXPTP_PANDA, PointerTo<NamedNode>)
EXPORT_TEMPLATE_CLASS(EXPCL_PANDA, EXPTP_PANDA, ConstPointerTo<NamedNode>)

typedef PointerTo<NamedNode> PT_NamedNode;
typedef ConstPointerTo<NamedNode> CPT_NamedNode;

// Tell GCC that we'll take care of the instantiation explicitly here.
#ifdef __GNUC__
#pragma interface
#endif

#endif
