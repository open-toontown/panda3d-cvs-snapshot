// Filename: colorBlendAttrib.h
// Created by:  drose (29Mar02)
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

#ifndef COLORBLENDATTRIB_H
#define COLORBLENDATTRIB_H

#include "pandabase.h"
#include "luse.h"
#include "renderAttrib.h"

class FactoryParams;

////////////////////////////////////////////////////////////////////
//       Class : ColorBlendAttrib
// Description : This specifies how colors are blended into the frame
//               buffer, for special effects.  This overrides
//               transparency if transparency is also specified.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_PGRAPH ColorBlendAttrib : public RenderAttrib {
PUBLISHED:
  enum Mode {
    M_none,             // Blending is disabled
    M_add,              // incoming color * A + fbuffer color * B
    M_subtract,         // incoming color * A - fbuffer color * B
    M_inv_subtract,     // fbuffer color * B - incoming color * A
    M_min,              // min(incoming color, fbuffer color)
    M_max               // max(incoming color, fbuffer color)
  };

  enum Operand {
    O_zero,
    O_one,
    O_incoming_color,
    O_one_minus_incoming_color,
    O_fbuffer_color,
    O_one_minus_fbuffer_color,
    O_incoming_alpha,
    O_one_minus_incoming_alpha,
    O_fbuffer_alpha,
    O_one_minus_fbuffer_alpha,
    O_constant_color,
    O_one_minus_constant_color,
    O_constant_alpha,
    O_one_minus_constant_alpha,
    O_incoming_color_saturate,  // valid only for operand a

    // If you set either of the operands to any of the below, the
    // blend color is taken from the current ColorScaleAttrib.  This
    // also inhibits the normal behavior of the ColorScaleAttrib; it
    // no longer directly scales the vertex colors, on the assumption
    // that you will instead take care of the scale here, in the blend
    // mode.
    O_color_scale,
    O_one_minus_color_scale,
    O_alpha_scale,
    O_one_minus_alpha_scale,
  };

private:
  INLINE ColorBlendAttrib();
  INLINE ColorBlendAttrib(Mode mode, Operand a, Operand b,
                          const Colorf &color);

PUBLISHED:
  static CPT(RenderAttrib) make_off();
  static CPT(RenderAttrib) make(Mode mode);
  static CPT(RenderAttrib) make(Mode mode, Operand a, Operand b,
                                const Colorf &color = Colorf::zero());
  static CPT(RenderAttrib) make_default();

  INLINE Mode get_mode() const;
  INLINE Operand get_operand_a() const;
  INLINE Operand get_operand_b() const;
  INLINE Colorf get_color() const;

  INLINE bool involves_constant_color() const;
  INLINE bool involves_color_scale() const;

  INLINE static bool involves_constant_color(Operand operand);
  INLINE static bool involves_color_scale(Operand operand);

public:
  virtual void output(ostream &out) const;
  virtual void store_into_slot(AttribSlots *slots) const;

protected:
  virtual int compare_to_impl(const RenderAttrib *other) const;

private:
  Mode _mode;
  Operand _a, _b;
  Colorf _color;
  bool _involves_constant_color;
  bool _involves_color_scale;

PUBLISHED:
  static int get_class_slot() {
    return _attrib_slot;
  }
  virtual int get_slot() const {
    return get_class_slot();
  }

public:
  static void register_with_read_factory();
  virtual void write_datagram(BamWriter *manager, Datagram &dg);

protected:
  static TypedWritable *make_from_bam(const FactoryParams &params);
  void fillin(DatagramIterator &scan, BamReader *manager);
  
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    RenderAttrib::init_type();
    register_type(_type_handle, "ColorBlendAttrib",
                  RenderAttrib::get_class_type());
    _attrib_slot = register_slot(_type_handle, 100, make_default);
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
  static int _attrib_slot;
};

ostream &operator << (ostream &out, ColorBlendAttrib::Mode mode);
ostream &operator << (ostream &out, ColorBlendAttrib::Operand operand);

#include "colorBlendAttrib.I"

#endif

