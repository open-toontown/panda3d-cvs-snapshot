// Filename: preparedGraphicsObjects.h
// Created by:  drose (19Feb04)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//
////////////////////////////////////////////////////////////////////

#ifndef PREPAREDGRAPHICSOBJECTS_H
#define PREPAREDGRAPHICSOBJECTS_H

#include "pandabase.h"
#include "referenceCount.h"
#include "texture.h"
#include "pStatCollector.h"
#include "pset.h"
#include "pmutex.h"

class TextureContext;
class GeomContext;
class GeomNodeContext;
class GraphicsStateGuardianBase;

////////////////////////////////////////////////////////////////////
//       Class : PreparedGraphicsObjects
// Description : A table of objects that are saved within the graphics
//               context for reference by handle later.  Generally,
//               this represents things like OpenGL texture objects or
//               display lists.
//
//               This object simply records the pointers to the
//               context objects created by the individual GSG's;
//               these context objects will contain enough information
//               to reference or release the actual object stored
//               within the graphics context.
//
//               These tables may potentially be shared between
//               related graphics contexts, hence their storage here
//               in a separate object rather than as a part of the
//               GraphicsStateGuardian.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA PreparedGraphicsObjects : public ReferenceCount {
public:
  PreparedGraphicsObjects();
  ~PreparedGraphicsObjects();

  void enqueue_texture(Texture *tex);
  bool dequeue_texture(Texture *tex);
  void release_texture(TextureContext *tc);
  int release_all_textures();

  TextureContext *prepare_texture_now(Texture *tex, GraphicsStateGuardianBase *gsg);
  void update(GraphicsStateGuardianBase *gsg);

private:
  typedef pset<TextureContext *> Textures;
  typedef pset< PT(Texture) > EnqueuedTextures;

  Mutex _lock;
  Textures _prepared_textures, _released_textures;  
  EnqueuedTextures _enqueued_textures;

  static PStatCollector _total_texusage_pcollector;

  friend class GraphicsStateGuardian;
};

#include "preparedGraphicsObjects.I"

#endif
