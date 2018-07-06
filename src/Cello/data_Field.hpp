// See LICENSE_CELLO file for license and copyright information

/// @file     data_Field.hpp
/// @author   James Bordner (jobordner@ucsd.edu)
/// @date     Mon Sep  8 14:16:32 PDT 2014
/// @brief    [\ref Data] Declaration of the Field class
///
/// The Field class is used to unify the interface of the global FieldDescr
/// object and a given FieldData object.

#ifndef DATA_FIELD_HPP
#define DATA_FIELD_HPP

class Field {

  /// @class    Field
  /// @ingroup  Data
  /// @brief    [\ref Data] 

public: // interface

  /// Constructor
  Field(FieldDescr * field_descr,
	FieldData * field_data) throw()
    : field_descr_ (field_descr),
      field_data_ (field_data)
  {}

  /// Copy constructor
  Field(const Field & field) throw()
  {
    field_descr_ = field.field_descr_;
    field_data_ = field.field_data_; 
  }

  /// Assignment operator
  Field & operator= (const Field & field) throw()
  { 
    field_descr_ = field.field_descr_;
    field_data_ = field.field_data_;
    return *this;
  }

  /// Destructor
  ~Field() throw()
  {};

  /// CHARM++ Pack / Unpack function
  void pup (PUP::er &p)
  {
    WARNING ("Field::pup()",
	     "Skipping since Field is intended as transient objects");
  };
  
  /// Return the field descriptor for this field
  FieldDescr * field_descr() { return field_descr_; }

  /// Return the field data for this field
  FieldData * field_data() { return field_data_; }

  void set_field_data(FieldData * field_data) 
  { field_data_ = field_data; }

  void set_field_descr(FieldDescr * field_descr) 
  { field_descr_ = field_descr; }

  //==================================================
  // FieldDescr
  //==================================================

  /// Set alignment
  void set_alignment(int alignment) throw()
  { field_descr_->set_alignment(alignment); }

  /// Set padding
  void set_padding(int padding) throw()
  { field_descr_->set_padding(padding); }

  /// Set centering for a field
  void set_centering(int id, int cx, int cy=0, int cz=0) 
    throw()
  { field_descr_->set_centering(id,cx,cy,cz); }

  /// Set ghost_depth for a field
  void set_ghost_depth(int id, int gx, int gy=0, int gz=0) 
    throw()
  { field_descr_->set_ghost_depth(id,gx,gy,gz); }


  /// Set precision for a field
  void set_precision(int id, int precision) 
    throw()
  { field_descr_->set_precision(id,precision); }

  /// Insert a new field
  int insert_permanent(const std::string & name) throw()
  { return field_descr_->insert_permanent(name); }

  /// Insert a new field
  int insert_temporary(const std::string & name = "") throw()
  { return field_descr_->insert_temporary(name); }

  /// Return the number of fields
  int field_count() const throw()
  { return field_descr_->field_count(); }

  /// Return name of the ith field
  std::string field_name(int id) const throw()
  { return field_descr_->field_name(id); }

  /// Return whether the field has been inserted
  bool is_field(const std::string & name) const throw()
  { return field_descr_->is_field(name); }

  /// Return the integer handle for the named field
  int field_id(const std::string & name) const throw()
  { return field_descr_->field_id(name); }

  //----------------------------------------------------------------------
  // Properties
  //----------------------------------------------------------------------

  Grouping * groups () 
  { return field_descr_->groups(); }

  const Grouping * groups () const 
  { return field_descr_->groups(); }

  /// alignment in bytes of fields in memory
  int alignment() const throw()
  { return field_descr_-> alignment();}

  /// padding in bytes between fields in memory
  int padding() const throw()
  { return field_descr_->padding() ;}

  /// centering of given field
  void centering(int id, int * cx, int * cy = 0, int * cz = 0) const 
    throw()
  { return field_descr_->centering(id,cx,cy,cz); }

  /// return whether the field variable is centered in the cell
  bool is_centered(int id) const
  { return field_descr_->is_centered(id); }

  /// depth of ghost zones of given field
  void ghost_depth(int id, int * gx, int * gy = 0, int * gz = 0) const 
    throw()
  { return field_descr_->ghost_depth(id,gx,gy,gz); }

  /// Return precision of given field
  int precision(int id) const throw()
  { return field_descr_->precision(id); }

  /// Number of bytes per element required by the given field
  int bytes_per_element(int id) const throw()
  { return field_descr_->bytes_per_element(id); }

  /// Whether the field is permanent or temporary
  bool is_permanent (int id_field) const throw()
  { return field_descr_->is_permanent(id_field); }

  /// Return the number of permanent fields
  int num_permanent() const throw()
  { return field_descr_->num_permanent(); }

  //--------------------------------------------------
  // History operations
  //--------------------------------------------------

  /// Set the history depth for storing old field values
  void set_history (int num_history)
  {
    field_descr_->set_history (num_history);
    field_data_ ->set_history_(field_descr_);
  }

  /// Return the number of history generations to store
  int num_history () const
  { return field_descr_->num_history(); }

  /// Copy "current" fields to history = 1 fields (saving time), and push
  /// back older generations up to num_history()
  void save_history (double time)
  { field_data_->save_history(field_descr_,time); }
  
  /// Return time for given history
  double history_time (int ih) const
  { return field_data_->history_time (field_descr_,ih); }

  //----------------------------------------------------------------------
  // Units operations
  //----------------------------------------------------------------------

  /// scale the field to cgs units given the unit scaling factor
  /// if it's already in cgs, then leave as-is
  /// except if it's in cgs but the scaling factor has changed (e.g. due to
  /// expansion) then adjust for the new scaling factor
  void units_scale_cgs (int id, double amount)
  { field_data_->units_scale_cgs (field_descr_,id,amount); }
    
  /// convert the field to "code units" given the unit scaling factor
  /// if it's already in code units, leave it as-is
  /// warning if scaling factor has changed
  void units_scale_code (int id, double amount)
  { field_data_->units_scale_code (field_descr_,id,amount); }

  /// Return the current scaling factor of the given Field
  /// 1.0 if in code units, or the scaling factor if in cgs
  double units_scaling (const FieldDescr *, int id)
  { return field_data_->units_scaling (field_descr_,id); }

  //==================================================
  // FieldData
  //==================================================

  /// Return size of fields on the data, assuming centered
  void size(int * nx, int * ny = 0, int * nz = 0) const throw()
  { field_data_->size(nx,ny,nz); }

  /// Return dimensions of fields on the data, assuming centered
  void dimensions(int id_field,int * mx, int * my = 0, int * mz = 0) const throw()
  { field_data_->dimensions(field_descr_,id_field, mx,my,mz); }

  /// Return array for the corresponding field, which may or may not
  /// contain ghosts depending on if they're allocated
  char * values (int id_field, int index_history=0) throw ()
  { return field_data_->values(field_descr_,id_field,index_history); }

  char * values (std::string name, int index_history=0) throw ()
  { return field_data_->values(field_descr_,name,index_history); }

  /// Return array for the corresponding field, which may or may not
  /// contain ghosts depending on if they're allocated
  const char * values (int id_field, int index_history=0) const throw ()
  { return field_data_->values(field_descr_,id_field,index_history); }

  const char * values (std::string name, int index_history=0) const throw ()
  { return field_data_->values(field_descr_,name,index_history); }

  /// Return array for the corresponding field, which does not contain
  /// ghosts whether they're allocated or not
  char * unknowns (int id_field, int index_history=0) throw ()
  { return field_data_->unknowns(field_descr_,id_field,index_history); }

  char * unknowns (std::string name, int index_history=0) throw ()
  { return field_data_->unknowns(field_descr_,name,index_history); }

  const char * unknowns (int id_field, int index_history=0) const throw ()
  { return field_data_->unknowns(field_descr_,id_field,index_history); }

  const char * unknowns (std::string name, int index_history=0) const throw ()
  { return field_data_->unknowns(field_descr_,name,index_history); }

  /// Return raw pointer to the array of all fields.  Const since
  /// otherwise dangerous due to varying field sizes, precisions,
  /// padding and alignment
  const char * permanent ()  const throw () 
  { return field_data_->permanent(); }

  /// Return width of cells along each dimension
  void cell_width(double xm,   double xp,   double * hx,
		  double ym=0, double yp=0, double * hy=0,
		  double zm=0, double zp=0, double * hz=0) const throw ()
  { field_data_->cell_width(xm,xp,hx,
			     ym,yp,hy,
			     zm,zp,hz); }

  /// Clear specified array(s) to specified value
  void clear ( float value = 0.0, 
	       int id_first = -1, 
	       int id_last  = -1) throw()
  { field_data_->clear (field_descr_,value,id_first,id_last); }
 
  /// Return whether array is allocated or not
  bool permanent_allocated() const throw()
  { return field_data_->permanent_allocated(); }

  /// Return whether array is allocated or not
  size_t permanent_size() const throw()
  { return field_data_->permanent_size(); }

  /// Allocate storage for the field data
  void allocate_permanent(bool ghosts_allocated = false) throw()
  { field_data_->allocate_permanent(field_descr_,ghosts_allocated); }

  /// Allocate storage for the temporary fields
  void allocate_temporary(int id) throw ()
  { field_data_->allocate_temporary(field_descr_,id); }

  /// Deallocate storage for the temporary fields
  void deallocate_temporary(int id) throw ()
  { field_data_->deallocate_temporary(field_descr_,id); }

  /// Reallocate storage for the field data, e.g. when changing
  /// from ghosts to non-ghosts [ costly for large blocks ]
  void reallocate_permanent(bool ghosts_allocated = false) throw()
  { field_data_->reallocate_permanent(field_descr_,ghosts_allocated); }

  /// Deallocate storage for the field data
  void deallocate_permanent() throw()
  { field_data_->deallocate_permanent(); }

  /// Return whether ghost cells are allocated or not.  
  bool ghosts_allocated() const throw ()
  { return field_data_->ghosts_allocated(); }

  /// Return the number of elements (nx,ny,nz) along each axis, and total
  /// number of bytes n
  int field_size (int id, int *nx=0, int *ny=0, int *nz=0) const throw()
  { return field_data_->field_size(field_descr_,id,nx,ny,nz); }

  //----------------------------------------------------------------------

  // BLAS Operations [depreciated]

  /// Compute inner product field(ix) . field(iy)
  double dot (int ix, int iy) throw()
  { return field_data_->dot (field_descr_,ix,iy); }
  
  /// Scale vector ix by scalar a
  void scale (int iy, long double a, int ix, bool ghosts = true ) throw()
  { field_data_->scale(field_descr_, iy,a,ix,ghosts); }

  //----------------------------------------------------------------------

  /// Print basic field characteristics for debugging
  void print (const char * message,
	      bool use_file = false) const throw()
  { field_data_->print(field_descr_,message,use_file); }

private: // attributes

  /// Field descriptor for global field data
  FieldDescr * field_descr_;

  /// Field data for the specific Block
  FieldData * field_data_;

  // NOTE: change pup() function whenever attributes change

};

#endif /* DATA_FIELD_HPP */
