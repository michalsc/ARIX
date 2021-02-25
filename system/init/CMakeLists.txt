project(init)
project_type(exe)

add_executable(init
    init
)

target_link_libraries(init PRIVATE 
    kernel.static)

target_link_options(init
    PRIVATE
        -static
        -nodefaultlibs
        -nostartfiles
)

add_dependencies(init
    kernel.includes
)

install(TARGETS init DESTINATION ARIX/System)

arix_finish(init)
