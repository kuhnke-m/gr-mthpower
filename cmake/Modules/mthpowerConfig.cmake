INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_MTHPOWER mthpower)

FIND_PATH(
    MTHPOWER_INCLUDE_DIRS
    NAMES mthpower/api.h
    HINTS $ENV{MTHPOWER_DIR}/include
        ${PC_MTHPOWER_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    MTHPOWER_LIBRARIES
    NAMES gnuradio-mthpower
    HINTS $ENV{MTHPOWER_DIR}/lib
        ${PC_MTHPOWER_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MTHPOWER DEFAULT_MSG MTHPOWER_LIBRARIES MTHPOWER_INCLUDE_DIRS)
MARK_AS_ADVANCED(MTHPOWER_LIBRARIES MTHPOWER_INCLUDE_DIRS)

