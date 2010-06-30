// Filename: colladaMaterial.h
// Created by: Xidram (20Apr10)
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

#ifndef COLLADAMATERIAL_H
#define COLLADAMATERIAL_H

#include "colladaAssetElement.h"
#include "colladaInstance.h"
#include "typedReferenceCount.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaMaterial
// Description : Object that represents the <material> COLLADA element.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaMaterial : public ColladaAssetElement {
PUBLISHED:
  INLINE ColladaMaterial(PT(ColladaInstanceEffect) effect = NULL);
  INLINE virtual void clear();

  virtual bool load_xml(const TiXmlElement *xelement);
  virtual TiXmlElement *make_xml() const;

  INLINE PT(ColladaInstanceEffect) get_instance_effect() const;
  INLINE void set_instance_effect(PT(ColladaInstanceEffect) instance_effect);

private:
  PT(ColladaInstanceEffect) _instance_effect;

public:
  // Needed by ColladaLibrary to validate the element names.
  static const string _element_name;
  static const string _library_name;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaAssetElement::init_type();
    register_type(_type_handle, "ColladaMaterial",
                  ColladaAssetElement::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "colladaMaterial.I"

#endif

