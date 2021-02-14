project(debug)
project_type(library)

list(APPEND GLOBAL_SYMBOLS
    DebugBase
)

add_library(debug SHARED
    debug_init
    vbug
)


import_library(libc "c")

target_link_libraries(debug PRIVATE libc)

install(TARGETS debug DESTINATION SYS/Libs)

arix_finish(debug)
