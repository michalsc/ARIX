project(kernel)
project_type(library)

list(APPEND GLOBAL_SYMBOLS
    KernelBase
)

add_library(kernel SHARED
    kernel_init sc_open sc_close sc_exit sc_write sc_read sc_mkdir sc_execve sc_mount
    sc_lseek sc_memfd_create sc_fcntl sc_clock_gettime sc_clock_settime sc_clock_getres
    sc_socket sc_bind sc_connect sc_ppoll sc_sched_yield sc_futex sc_mmap sc_munmap
    sc_sendto sc_sendmsg sc_recvfrom sc_recvmsg sc_clock_nanosleep sc_getpid sc_getppid
    sc_gettid sc_unshare sc_symlink sc_symlinkat sc_readlink sc_readlinkat sc_getrandom
    sc_prctl sc_clone
)

target_link_options(kernel
    PRIVATE
        -nostdlib
)

add_library(kernel.static STATIC
    sc_open sc_close sc_exit sc_write sc_read sc_mkdir sc_execve sc_mount
    sc_lseek sc_memfd_create sc_fcntl sc_clock_gettime sc_clock_settime sc_clock_getres
    sc_socket sc_bind sc_connect sc_ppoll sc_sched_yield sc_futex sc_mmap sc_munmap
    sc_sendto sc_sendmsg sc_recvfrom sc_recvmsg sc_clock_nanosleep sc_getpid sc_getppid
    sc_gettid sc_unshare sc_symlink sc_symlinkat sc_readlink sc_readlinkat sc_getrandom
    sc_prctl sc_clone
)
set_target_properties(kernel.static PROPERTIES OUTPUT_NAME kernel)

install(TARGETS kernel DESTINATION ARIX/Libs)
install(TARGETS kernel.static DESTINATION ARIX/Development/lib)

arix_finish(kernel)
