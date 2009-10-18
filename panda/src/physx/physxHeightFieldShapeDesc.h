// Filename: physxHeightFieldShapeDesc.h
// Created by:  enn0x (15Oct09)
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

#ifndef PHYSXHEIGHTFIELDSHAPEDESC_H
#define PHYSXHEIGHTFIELDSHAPEDESC_H

#include "pandabase.h"

#include "physxShapeDesc.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

class PhysxHeightField;

////////////////////////////////////////////////////////////////////
//       Class : PhysxHeightFieldShapeDesc
// Description : Descriptor class for PhysxHeightFieldShape.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxHeightFieldShapeDesc : public PhysxShapeDesc {

PUBLISHED:
  INLINE PhysxHeightFieldShapeDesc();
  INLINE ~PhysxHeightFieldShapeDesc();

  void set_to_default();
  INLINE bool is_valid() const;

  void set_height_field(const PhysxHeightField &hf);
  void set_dimensions(const LVector3f &dimensions);

public:
  INLINE PhysxHeightFieldShapeDesc(NxHeightFieldShapeDesc &desc);

  NxShapeDesc *ptr() const { return (NxShapeDesc *)&_desc; };

private:
  NxHeightFieldShapeDesc _desc;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PhysxShapeDesc::init_type();
    register_type(_type_handle, "PhysxHeightFieldShapeDesc", 
                  PhysxShapeDesc::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {
    init_type();
    return get_class_type();
  }

private:
  static TypeHandle _type_handle;
};

#include "physxHeightFieldShapeDesc.I"

#endif // PHYSXHEIGHTFIELDSHAPEDESC_H
