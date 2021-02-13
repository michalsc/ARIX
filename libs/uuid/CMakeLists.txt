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

add_dependencies(uuid 
    includes.uuid.library
    includes.exec.library
)

install(TARGETS uuid DESTINATION SYS/Libs)

arix_finish(uuid)
