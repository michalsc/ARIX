if(__ARIX_CMAKE_INCLUDED)
  return()
endif()
set(__ARIX_CMAKE_INCLUDED TRUE)

set(CMAKE_SHARED_LIBRARY_C_FLAGS "-fpic")
set(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS "-shared")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_RUNTIME_C_FLAG "")
set(CMAKE_SHARED_LIBRARY_RUNTIME_C_FLAG_SEP "")

set(CMAKE_LINK_LIBRARY_SUFFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_SHARED_LIBRARY_PREFIX "")          # lib
set(CMAKE_SHARED_LIBRARY_SUFFIX ".library")           # .a
set(CMAKE_EXECUTABLE_SUFFIX "")             # same suffix as if built using UseEcos.cmake
set(CMAKE_DL_LIBS "" )

set(CMAKE_FIND_LIBRARY_PREFIXES "")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".library")

include(Platform/UnixPaths)

add_definitions(-D__ARIX__=1 -D__ARIX=1)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

set(CMAKE_CXX_LINK_EXECUTABLE  "<CMAKE_CXX_COMPILER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> -Wl,-unresolved-symbols=ignore-in-shared-libs <LINK_LIBRARIES>")
set(CMAKE_C_LINK_EXECUTABLE    "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> -Wl,-unresolved-symbols=ignore-in-shared-libs <LINK_LIBRARIES>")
set(CMAKE_CXX_CREATE_SHARED_LIBRARY "<CMAKE_CXX_COMPILER> -shared -nodefaultlibs <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> -Wl,-soname,<TARGET> -Wl,--version-script=<CMAKE_CURRENT_BINARY_DIR>/version.script -fvisibility=hidden <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_C_CREATE_SHARED_LIBRARY "<CMAKE_C_COMPILER> -shared -nodefaultlibs <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> -Wl,-soname,<TARGET> -Wl,--version-script=<CMAKE_CURRENT_BINARY_DIR>/version.script -fvisibility=hidden <OBJECTS> -o <TARGET> <LINK_LIBRARIES>  ")

