project(dos)
project_type(library)

list(APPEND GLOBAL_SYMBOLS
    DOSBase
)

add_library(dos SHARED
    dos_init setioerr ioerr open close read write assignpath arixfile path dos_debug setcomment
)

add_dependencies(dos
    exec.includes
)

import_library(libc "c")
import_library(cpp "stdc++")

target_link_libraries(dos PRIVATE 
    libc cpp exec)

install(TARGETS dos DESTINATION SYS/Libs)

arix_finish(dos)
