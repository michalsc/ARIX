project(debug)
project_type(library)

list(APPEND GLOBAL_SYMBOLS
    DebugBase
)

add_library(debug SHARED
    debug_init
    vbug
)

add_dependencies(debug 
    includes.debug.library
)

target_link_libraries(debug PUBLIC c uuid)

install(TARGETS debug DESTINATION SYS/Libs)

arix_finish(debug)
