# - Find Xinerama
# Find the Xinerama libraries
#
#  This module defines the following variables:
#     XINERAMA_FOUND        - 1 if XINERAMA_INCLUDE_DIR & XINERAMA_LIBRARY are found, 0 otherwise
#     XINERAMA_INCLUDE_DIR  - where to find Xlib.h, etc.
#     XINERAMA_LIBRARY      - the X11 library
#

find_path( XINERAMA_INCLUDE_DIR
           NAMES X11/extensions/Xinerama.h
           PATH_SUFFIXES X11/extensions
           DOC "The XINERAMA include directory" )

find_library( XINERAMA_LIBRARY
              NAMES Xinerama
              PATHS /usr/lib /lib
              DOC "The Xinerama library" )

if( XINERAMA_INCLUDE_DIR AND XINERAMA_LIBRARY )
    set( XINERAMA_FOUND 1 )
else()
    set( XINERAMA_FOUND 0 )
endif()

mark_as_advanced( XINERAMA_INCLUDE_DIR XINERAMA_LIBRARY )