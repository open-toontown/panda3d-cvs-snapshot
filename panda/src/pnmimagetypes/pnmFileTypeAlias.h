// Filename: pnmFileTypeAlias.h
// Created by:  drose (17Jun00)
// 
////////////////////////////////////////////////////////////////////

#ifndef PNMFILETYPEALIAS_H
#define PNMFILETYPEALIAS_H

#include <pandabase.h>

#include <pnmFileType.h>
#include <pnmReader.h>
#include <pnmWriter.h>

////////////////////////////////////////////////////////////////////
//       Class : PNMFileTypeAlias
// Description : For reading and Alias native image files.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA PNMFileTypeAlias : public PNMFileType {
public:
  PNMFileTypeAlias();

  virtual string get_name() const;

  virtual int get_num_extensions() const;
  virtual string get_extension(int n) const;
  virtual string get_suggested_extension() const;

  virtual PNMReader *make_reader(FILE *file, bool owns_file = true,
                                 const string &magic_number = string());
  virtual PNMWriter *make_writer(FILE *file, bool owns_file = true);

public:
  class Reader : public PNMReader {
  public:
    Reader(PNMFileType *type, FILE *file, bool owns_file, string magic_number);

    virtual bool supports_read_row() const;
    virtual bool read_row(xel *array, xelval *alpha);
  };

  class Writer : public PNMWriter {
  public:
    Writer(PNMFileType *type, FILE *file, bool owns_file);

    virtual bool supports_write_row() const;
    virtual bool write_header();
    virtual bool write_row(xel *array, xelval *alpha);
  };


  // The TypedWritable interface follows.
public:
  static void register_with_read_factory();

protected:
  static TypedWritable *make_PNMFileTypeAlias(const FactoryParams &params);

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PNMFileType::init_type();
    register_type(_type_handle, "PNMFileTypeAlias",
                  PNMFileType::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}
 
private:
  static TypeHandle _type_handle;
};

#endif

  
