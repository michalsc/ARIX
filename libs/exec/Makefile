OBJNAME   := exec
OBJTYPE   := library
TARGETDIR := Libs
LDLIBS    := c dl stdc++
GLOBAL    := LibBase ExecBase
LOCAL     := *
FILES     := openlibrary allocmem freemem reschedule addhead addtail insert remhead remtail enqueue \
             findname remove closelibrary exec_init allocvec freevec availmem spawn copymem tlsf \
             createpool deletepool allocpooled freepooled reallocmem allocmemaligned allocvecaligned \
             getsymboladdress allocabs avl reallocpooled getmsg waitport putmsg createmsgport \
             replymsg discardmsg findport addport deletemsgport remport testport getrandomid \
             createthread tags exec_debug createmutex deletemutex obtainmutex releasemutex initmutex \
             attemptmutex createiorequest deleteiorequest doio waitio sendio checkio abortio \
             exec_random getid

include $(ROOT_DIR)/config/make_shared.mk
include $(ROOT_DIR)/config/make_bottom.mk
