// Filename: texturePool.cxx
// Created by:  drose (26Apr00)
// 
////////////////////////////////////////////////////////////////////

#include "texturePool.h"
#include "config_gobj.h"
#include "config_util.h"


TexturePool *TexturePool::_global_ptr = (TexturePool *)NULL;


////////////////////////////////////////////////////////////////////
//     Function: TexturePool::ns_has_texture
//       Access: Private
//  Description: The nonstatic implementation of has_texture().
////////////////////////////////////////////////////////////////////
bool TexturePool::
ns_has_texture(Filename filename) {
  filename.resolve_filename(get_texture_path());
  filename.resolve_filename(get_model_path());

  Textures::const_iterator ti;
  ti = _textures.find(filename);
  if (ti != _textures.end()) {
    // This texture was previously loaded.
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePool::ns_load_texture
//       Access: Private
//  Description: The nonstatic implementation of load_texture().
////////////////////////////////////////////////////////////////////
Texture *TexturePool::
ns_load_texture(Filename filename) {
  filename.resolve_filename(get_texture_path());
  filename.resolve_filename(get_model_path());

  Textures::const_iterator ti;
  ti = _textures.find(filename);
  if (ti != _textures.end()) {
    // This texture was previously loaded.
    return (*ti).second;
  }

  gobj_cat.info()
    << "Loading texture " << filename << "\n";
  PT(Texture) tex = new Texture;
  if (!tex->read(filename)) {
    // This texture was not found.
    gobj_cat.error() << "Unable to read texture " << filename << "\n";
    return NULL;
  }

  _textures[filename] = tex;
  return tex;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePool::ns_load_texture
//       Access: Private
//  Description: The nonstatic implementation of load_texture().
////////////////////////////////////////////////////////////////////
Texture *TexturePool::
ns_load_texture(Filename filename, Filename grayfilename) {
  filename.resolve_filename(get_texture_path());
  filename.resolve_filename(get_model_path());

  grayfilename.resolve_filename(get_texture_path());
  grayfilename.resolve_filename(get_model_path());

  Textures::const_iterator ti;
  ti = _textures.find(filename);
  if (ti != _textures.end()) {
    // This texture was previously loaded.
    return (*ti).second;
  }

  gobj_cat.info()
    << "Loading texture " << filename << " and grayscale texture "
    << grayfilename << endl;
  PT(Texture) tex = new Texture;
  if (!tex->read(filename, grayfilename)) {
    // This texture was not found.
    gobj_cat.error() << "Unable to read texture " << filename << "\n";
    return NULL;
  }

  _textures[filename] = tex;
  return tex;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePool::ns_add_texture
//       Access: Private
//  Description: The nonstatic implementation of add_texture().
////////////////////////////////////////////////////////////////////
void TexturePool::
ns_add_texture(Texture *tex) {
  string filename = tex->get_name();
  if (filename.empty()) {
    gobj_cat.error() << "Attempt to call add_texture() on an unnamed texture.\n";
  }

  // We blow away whatever texture was there previously, if any.
  _textures[filename] = tex;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePool::ns_release_texture
//       Access: Private
//  Description: The nonstatic implementation of release_texture().
////////////////////////////////////////////////////////////////////
void TexturePool::
ns_release_texture(Texture *tex) {
  string filename = tex->get_name();
  Textures::iterator ti;
  ti = _textures.find(filename);
  if (ti != _textures.end() && (*ti).second == tex) {
    _textures.erase(ti);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePool::ns_release_all_textures
//       Access: Private
//  Description: The nonstatic implementation of release_all_textures().
////////////////////////////////////////////////////////////////////
void TexturePool::
ns_release_all_textures() {
  _textures.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePool::ns_garbage_collect
//       Access: Private
//  Description: The nonstatic implementation of garbage_collect().
////////////////////////////////////////////////////////////////////
int TexturePool::
ns_garbage_collect() {
  int num_released = 0;
  Textures new_set;

  Textures::iterator ti;
  for (ti = _textures.begin(); ti != _textures.end(); ++ti) {
    Texture *tex = (*ti).second;
    if (tex->get_ref_count() == 1) {
      if (gobj_cat.is_debug()) {
	gobj_cat.debug()
	  << "Releasing " << (*ti).first << "\n";
      }
      num_released++;
    } else {
      new_set.insert(new_set.end(), *ti);
    }
  }

  _textures.swap(new_set);
  return num_released;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePool::ns_list_contents
//       Access: Private
//  Description: The nonstatic implementation of list_contents().
////////////////////////////////////////////////////////////////////
void TexturePool::
ns_list_contents(ostream &out) {
  out << _textures.size() << " textures:\n";
  Textures::iterator ti;
  for (ti = _textures.begin(); ti != _textures.end(); ++ti) {
    out << "  " << (*ti).first
	<< " (count = " << (*ti).second->get_ref_count() << ")\n";
  }
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePool::get_ptr
//       Access: Private, Static
//  Description: Initializes and/or returns the global pointer to the
//               one TexturePool object in the system.
////////////////////////////////////////////////////////////////////
TexturePool *TexturePool::
get_ptr() {
  if (_global_ptr == (TexturePool *)NULL) {
    _global_ptr = new TexturePool;
  }
  return _global_ptr;
}
