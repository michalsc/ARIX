project(uuid)
project_type(library)

list(APPEND GLOBAL_SYMBOLS
    UUIDBase
)

add_library(uuid SHARED
    uuid_clear
    uuid_copy
    uuid_init
)

install(TARGETS uuid DESTINATION ARIX/Libs)

arix_finish(uuid)
