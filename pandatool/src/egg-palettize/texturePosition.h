// Filename: texturePosition.h
// Created by:  drose (04Dec00)
// 
////////////////////////////////////////////////////////////////////

#ifndef TEXTUREPOSITION_H
#define TEXTUREPOSITION_H

#include <pandatoolbase.h>

#include <typedWriteable.h>
#include <luse.h>
#include <eggTexture.h>

////////////////////////////////////////////////////////////////////
// 	 Class : TexturePosition
// Description : This represents a particular position of a texture
//               within a PaletteImage.  There is only one of these
//               per TexturePlacement, but it exists as a separate
//               structure so the TexturePlacement can easily consider
//               repositioning the texture.
////////////////////////////////////////////////////////////////////
class TexturePosition : public TypedWriteable {
public:
  TexturePosition();
  TexturePosition(const TexturePosition &copy);
  void operator = (const TexturePosition &copy);

  int _margin;
  int _x, _y;
  int _x_size, _y_size;

  TexCoordd _min_uv;
  TexCoordd _max_uv;

  EggTexture::WrapMode _wrap_u;
  EggTexture::WrapMode _wrap_v;

  // The TypedWriteable interface follows.
public:
  static void register_with_read_factory();
  virtual void write_datagram(BamWriter *writer, Datagram &datagram); 

protected:
  static TypedWriteable *make_TexturePosition(const FactoryParams &params);

public:
  void fillin(DatagramIterator &scan, BamReader *manager);

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedWriteable::init_type();
    register_type(_type_handle, "TexturePosition",
		  TypedWriteable::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }

private:
  static TypeHandle _type_handle;
};

#endif

