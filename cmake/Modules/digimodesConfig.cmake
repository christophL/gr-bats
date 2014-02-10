INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_DIGIMODES digimodes)

FIND_PATH(
    DIGIMODES_INCLUDE_DIRS
    NAMES digimodes/api.h
    HINTS $ENV{DIGIMODES_DIR}/include
        ${PC_DIGIMODES_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREEFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    DIGIMODES_LIBRARIES
    NAMES gnuradio-digimodes
    HINTS $ENV{DIGIMODES_DIR}/lib
        ${PC_DIGIMODES_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DIGIMODES DEFAULT_MSG DIGIMODES_LIBRARIES DIGIMODES_INCLUDE_DIRS)
MARK_AS_ADVANCED(DIGIMODES_LIBRARIES DIGIMODES_INCLUDE_DIRS)

