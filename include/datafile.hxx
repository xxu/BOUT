/*!
 * \file datafile.hxx
 *
 * \brief Data file handling object definition
 *
 * \author B.Dudson
 * \date   April 2009
 *
 * 26th Sep 2009: Modified to use varargs
 */

class Datafile;

#ifndef __DATAFILE_H__
#define __DATAFILE_H__

#include "bout_types.hxx"
#include "field2d.hxx"
#include "field3d.hxx"
#include "vector2d.hxx"
#include "vector3d.hxx"

#include "dataformat.hxx"

#include <stdarg.h>
#include <stdio.h>

#include <vector>
#include <string>

#ifndef DATAFILE_ORIGIN
extern char DEFAULT_FILE_EXT[]; ///< Default file extension
#endif

/// Work out which data format to use for given filename
/// Omit argument (or pass NULL) for default format
DataFormat *data_format(const char *filename = NULL);

/*!
  Uses a generic interface to file formats (DataFormat)
  and provides an interface for reading/writing simulation data.
  
  Data formats are currently implemented for PDB and netCDF.
*/
class Datafile {
 public:
  Datafile();
  Datafile(DataFormat *format);
  ~Datafile();
  
  /// Set the file format by passing an interface class
  void setFormat(DataFormat *format);

  void setLowPrecision(); ///< Only output floats

  void add(int &i, const char *name, int grow = 0);
  void add(BoutReal &r, const char *name, int grow = 0);
  void add(Field2D &f, const char *name, int grow = 0);
  void add(Field3D &f, const char *name, int grow = 0);
  void add(Vector2D &f, const char *name, int grow = 0);
  void add(Vector3D &f, const char *name, int grow = 0);

  /// Read a given file into the added variables
  int read(const char *filename, ...);
  int read(const string &filename) {return read(filename.c_str());}
  int read() {return read(def_filename); }
  /// Write the variables to the given file (over-writes existing file)
  int write(const char *filename, ...);
  int write() {return write(def_filename.c_str()); }
  /// Append data to an existing file (error if doesn't exist)
  int append(const char *filename, ...);
  int append() {return append(def_filename.c_str()); }
  
  bool write(const string &filename, bool append=false);
  bool write(bool append) {return write(def_filename, append); }
  
  /// Set a default filename 
  void setFilename(const char *format, ...);
  void setFilename(const string &filename) { def_filename = filename; }

  /// Set this to false to switch off all data writing
  static bool enabled;
  
  static BoutReal wtime; ///< Keep track of wall-time used
 private:
  string def_filename; ///< Default filename
  
  bool low_prec;

  DataFormat *file;

  /// A structure to hold a pointer to a class, and associated name and flags
  template <class T>
    struct VarStr {
      T *ptr;
      string name;
      bool grow;
      bool covar;
    };

  // one set per variable type
  vector< VarStr<int> >      int_arr;
  vector< VarStr<BoutReal> >     BoutReal_arr;
  vector< VarStr<Field2D> >  f2d_arr;
  vector< VarStr<Field3D> >  f3d_arr;
  vector< VarStr<Vector2D> > v2d_arr;
  vector< VarStr<Vector3D> > v3d_arr;

  bool read_f2d(const string &name, Field2D *f, bool grow);
  bool read_f3d(const string &name, Field3D *f, bool grow);

  bool write_f2d(const string &name, Field2D *f, bool grow);
  bool write_f3d(const string &name, Field3D *f, bool grow);

  bool varAdded(const string &name); // Check if a variable has already been added
};

#endif // __DATAFILE_H__
