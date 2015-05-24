INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_SCFDE scfde)

FIND_PATH(
    SCFDE_INCLUDE_DIRS
    NAMES scfde/api.h
    HINTS $ENV{SCFDE_DIR}/include
        ${PC_SCFDE_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    SCFDE_LIBRARIES
    NAMES gnuradio-scfde
    HINTS $ENV{SCFDE_DIR}/lib
        ${PC_SCFDE_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SCFDE DEFAULT_MSG SCFDE_LIBRARIES SCFDE_INCLUDE_DIRS)
MARK_AS_ADVANCED(SCFDE_LIBRARIES SCFDE_INCLUDE_DIRS)

