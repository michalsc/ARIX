
add_custom_target(includes ALL)

add_custom_command(
    TARGET includes PRE_BUILD
    COMMAND rsync -am 
        ${CMAKE_CURRENT_SOURCE_DIR}/sys_include/ ${CMAKE_BINARY_DIR}/sys_include
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/sys_include/ DESTINATION SYS/Development/include)
