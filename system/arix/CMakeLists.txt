project(ARIX)
project_type(exe)

add_executable(ARIX
    arix_start publicport
)

import_library(cpp "stdc++")

target_link_libraries(ARIX PRIVATE 
    exec cpp)

add_dependencies(init
    exec.includes
    kernel.includes
)

install(TARGETS ARIX DESTINATION ARIX/System)

arix_finish(ARIX)
