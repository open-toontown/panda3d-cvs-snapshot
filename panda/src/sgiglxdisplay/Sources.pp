#define BUILD_DIRECTORY $[and $[HAVE_SGIGL],$[HAVE_GLX]]

#define OTHER_LIBS interrogatedb:c dconfig:c dtoolconfig:m \
                   dtoolutil:c dtoolbase:c dtool:m

#begin lib_target
  #define TARGET sgiglxdisplay
  #define LOCAL_LIBS \
    sgidisplay glxdisplay

  #define SOURCES \
    config_sgiglxdisplay.cxx config_sgiglxdisplay.h \
    sgiglxGraphicsPipe.cxx sgiglxGraphicsPipe.h

  #define INSTALL_HEADERS \
    sgiglxGraphicsPipe.h

#end lib_target

