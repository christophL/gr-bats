INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_BATS bats)

FIND_PATH(
    BATS_INCLUDE_DIRS
    NAMES bats/api.h
    HINTS $ENV{BATS_DIR}/include
        ${PC_BATS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREEFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    BATS_LIBRARIES
    NAMES gnuradio-bats
    HINTS $ENV{BATS_DIR}/lib
        ${PC_BATS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BATS DEFAULT_MSG BATS_LIBRARIES BATS_INCLUDE_DIRS)
MARK_AS_ADVANCED(BATS_LIBRARIES BATS_INCLUDE_DIRS)

