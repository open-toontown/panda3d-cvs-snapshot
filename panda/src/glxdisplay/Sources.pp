#define BUILD_DIRECTORY $[HAVE_GLX]

#define OTHER_LIBS interrogatedb:c dconfig:c dtoolconfig:m \
                   dtoolutil:c dtoolbase:c dtool:m
#define USE_PACKAGES gl glx cg
#if $[LINK_IN_GLXGETPROCADDRESS]
  #define EXTRA_CDEFS LINK_IN_GLXGETPROCADDRESS
#endif

#begin lib_target
  #define TARGET glxdisplay
  #define LOCAL_LIBS \
    glgsg

  #define SOURCES \
    config_glxdisplay.cxx config_glxdisplay.h \
    glxGraphicsBuffer.h glxGraphicsBuffer.I glxGraphicsBuffer.cxx \
    glxGraphicsPipe.I glxGraphicsPipe.cxx glxGraphicsPipe.h \
    glxGraphicsWindow.h glxGraphicsWindow.I glxGraphicsWindow.cxx \
    glxGraphicsStateGuardian.h glxGraphicsStateGuardian.I \
    glxGraphicsStateGuardian.cxx \
    glxext.h

  #define INSTALL_HEADERS \
    glxGraphicsBuffer.I glxGraphicsBuffer.h \
    glxGraphicsPipe.I glxGraphicsPipe.h \
    glxGraphicsWindow.I glxGraphicsWindow.h

#end lib_target

