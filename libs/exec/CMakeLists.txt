project(exec)
project_type(library)

list(APPEND GLOBAL_SYMBOLS
    ExecBase
)

add_library(exec SHARED
    openlibrary allocmem freemem reschedule addhead addtail insert remhead remtail enqueue
    findname remove closelibrary exec_init allocvec freevec availmem spawn copymem tlsf
    createpool deletepool allocpooled freepooled reallocmem allocmemaligned allocvecaligned
    getsymboladdress allocabs avl reallocpooled getmsg waitport putmsg createmsgport
    replymsg discardmsg findport addport deletemsgport remport testport getrandomid
    createthread tags exec_debug createmutex deletemutex obtainmutex releasemutex initmutex
    attemptmutex createiorequest deleteiorequest doio waitio sendio checkio abortio
    exec_random getid
)

add_dependencies(exec
    kernel.includes
)

import_library(libc "c")
import_library(dl "dl")
import_library(cpp "stdc++")

target_link_libraries(exec PRIVATE 
    libc dl cpp kernel)

install(TARGETS exec DESTINATION ARIX/Libs)

arix_finish(exec)
