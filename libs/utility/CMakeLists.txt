project(utility)
project_type(library)

list(APPEND GLOBAL_SYMBOLS
    UtilityBase
)

add_library(utility SHARED
    utility_init callhookpkt allocatetagitems freetagitems applytagchanges clonetagitems
    filtertagchanges filtertagitems findtagitem gettagdata maptags nexttagitem packbooltags
    packstructuretags refreshtagitemclones taginarray unpackstructuretags
)

add_dependencies(utility 
    exec.includes
)

target_link_libraries(utility PRIVATE
    exec
)

install(TARGETS utility DESTINATION ARIX/Libs)

arix_finish(utility)
