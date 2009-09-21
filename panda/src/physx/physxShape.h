// Filename: physxShape.h
// Created by:  enn0x (16Sep09)
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

#ifndef PHYSXSHAPE_H
#define PHYSXSHAPE_H

#include "pandabase.h"
#include "pointerTo.h"
#include "lpoint3.h"
#include "lmatrix.h"
#include "lquaternion.h"

#include "physx_enumerations.h"
#include "physxObject.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

class PhysxActor;
class PhysxMaterial;

////////////////////////////////////////////////////////////////////
//       Class : PhysxShape
// Description : Abstract base class for shapes.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxShape : public PhysxObject {

PUBLISHED:
  INLINE PhysxShape();
  INLINE ~PhysxShape();

  PT(PhysxActor) get_actor() const;

  void set_name(const char *name);
  void set_flag(const PhysxShapeFlag flag, bool value);
  void set_skin_width(float skinWidth);
  void set_group(unsigned short group);
  void set_local_pos(const LPoint3f &pos);
  void set_local_mat(const LMatrix4f &mat);
  void set_material(const PhysxMaterial &material);
  void set_material_index(unsigned short idx);

  const char *get_name() const;
  bool get_flag(const PhysxShapeFlag flag) const;
  float get_skin_width() const;
  unsigned short get_group() const;
  LPoint3f get_local_pos() const;
  LMatrix4f get_local_mat() const;
  unsigned short get_material_index() const;

  //void get_world_bounds(PhysxBounds3 & dest) const;
  //bool check_overlap_aabb(const PhysxBounds3 & world_bounds) const;
  //bool check_overlap_capsule(const PhysxCapsule & world_capsule) const;
  //bool check_overlap_obb(const PhysxBox & world_box) const;
  //bool check_overlap_sphere(const PhysxSphere & world_sphere) const;

  //void set_groups_mask(const PhysxMask128 &mask);
  //PhysxMask128 get_groups_mask() const;


////////////////////////////////////////////////////////////////////
PUBLISHED:
  void release();

public:
  static PT(PhysxShape) factory(NxShapeType shapeType);

  virtual NxShape *ptr() const = 0;

  virtual void link(NxShape *shapePtr) = 0;
  virtual void unlink() = 0;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PhysxObject::init_type();
    register_type(_type_handle, "PhysxShape", 
                  PhysxObject::get_class_type());
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

#include "physxShape.I"

#endif // PHYSXSHAPE_H
