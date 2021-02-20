project(debug)
project_type(library)

list(APPEND GLOBAL_SYMBOLS
    DebugBase
)

add_library(debug SHARED
    debug_init
    vbug
)

target_link_libraries(debug PRIVATE kernel)

install(TARGETS debug DESTINATION ARIX/Libs)

arix_finish(debug)
