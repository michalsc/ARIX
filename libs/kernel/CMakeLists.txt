project(kernel)
project_type(library)

list(APPEND GLOBAL_SYMBOLS
    KernelBase
)

add_library(kernel SHARED
    kernel_init sc_open sc_close sc_exit sc_write sc_read sc_mkdir sc_execve sc_mount
    sc_lseek sc_memfd_create sc_fcntl sc_clock_gettime sc_clock_settime sc_clock_getres
    sc_socket sc_bind sc_connect sc_ppoll sc_sched_yield sc_futex sc_mmap sc_munmap
    sc_sendto sc_sendmsg sc_recvfrom sc_recvmsg sc_clock_nanosleep
)

add_library(kernel.static STATIC
    sc_open sc_close sc_exit sc_write sc_read sc_mkdir sc_execve sc_mount
    sc_lseek sc_memfd_create sc_fcntl sc_clock_gettime sc_clock_settime sc_clock_getres
    sc_socket sc_bind sc_connect sc_ppoll sc_sched_yield sc_futex sc_mmap sc_munmap
    sc_sendto sc_sendmsg sc_recvfrom sc_recvmsg sc_clock_nanosleep
)
set_target_properties(kernel.static PROPERTIES OUTPUT_NAME kernel)

install(TARGETS kernel DESTINATION SYS/Libs)
install(TARGETS kernel.static DESTINATION SYS/Development/lib)

arix_finish(kernel)
