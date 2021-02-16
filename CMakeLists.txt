cmake_minimum_required(VERSION 3.14.0)
project(ARIX VERSION 60.0)

include(cmake/verstring.cmake)
include(cmake/arix.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(SFDC ${CMAKE_SOURCE_DIR}/tools/sfdc/sfdc.py)

find_program(RSYNC rsync REQUIRED)
find_program(CPIO cpio REQUIRED)
find_program(GZIP gzip REQUIRED)
find_program(FIND find REQUIRED)

set(SUPPORTED_ARCHS "aarch64")
set(ARCH ${CMAKE_SYSTEM_PROCESSOR} CACHE STRING "One of target machines: ${SUPPORTED_ARCHS}")
set_property(CACHE ARCH PROPERTY STRINGS ${SUPPORTED_ARCHS})

if (${ARCH} IN_LIST SUPPORTED_ARCHS)
    message("-- Selected target arch: ${ARCH}")
else()
    message(FATAL_ERROR "Wrong target arch specified: ${ARCH}")
endif()

add_subdirectory(external/tiny-stl)
add_subdirectory(include)
add_subdirectory(libs)
add_subdirectory(system)
add_subdirectory(initrd)

install(SCRIPT cmake/initrd.cmake)
