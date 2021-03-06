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
    replymsg discardmsg findport addport deletemsgport remport testport getrandomuuid
    createthread tags exec_debug createmutex deletemutex obtainmutex releasemutex initmutex
    attemptmutex createiorequest deleteiorequest doio waitio sendio checkio abortio
    exec_random getid
)

add_dependencies(exec
    kernel.includes
)

import_library(dl "dl")
import_library(gcc "gcc")

target_link_libraries(exec PRIVATE 
    dl gcc kernel tinystl)

install(TARGETS exec DESTINATION ARIX/Libs)

arix_finish(exec)
