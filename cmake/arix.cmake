
function(project_type TYPE)
    set(PROJECT_TYPE ${TYPE} PARENT_SCOPE)
endfunction(project_type)


function(build_headers)
    add_custom_target(includes.${PROJECT_NAME}.${PROJECT_TYPE} ALL
                      DEPENDS ${CMAKE_BINARY_DIR}/sys_include/proto/${PROJECT_NAME}.h
                      DEPENDS ${CMAKE_BINARY_DIR}/sys_include/inline/${PROJECT_NAME}.h
                      DEPENDS ${CMAKE_BINARY_DIR}/sys_include/defines/${PROJECT_NAME}.h
                      DEPENDS ${CMAKE_BINARY_DIR}/sys_include/clib/${PROJECT_NAME}_protos.h
                      )

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/sys_include/proto/${PROJECT_NAME}.h
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/sys_include/proto/
        COMMAND ${SFDC} header_proto ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.json > ${CMAKE_BINARY_DIR}/sys_include/proto/${PROJECT_NAME}.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.json
        VERBATIM
    )

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/sys_include/clib/${PROJECT_NAME}_protos.h
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/sys_include/clib/
        COMMAND ${SFDC} header_clib ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.json > ${CMAKE_BINARY_DIR}/sys_include/clib/${PROJECT_NAME}_protos.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.json
        VERBATIM
    )

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/sys_include/inline/${PROJECT_NAME}.h
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/sys_include/inline/
        COMMAND ${SFDC} header_inline ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.json > ${CMAKE_BINARY_DIR}/sys_include/inline/${PROJECT_NAME}.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.json
        VERBATIM
    )

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/sys_include/defines/${PROJECT_NAME}.h
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/sys_include/defines/
        COMMAND ${SFDC} header_defines ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.json > ${CMAKE_BINARY_DIR}/sys_include/defines/${PROJECT_NAME}.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.json
        VERBATIM
    )

endfunction(build_headers)
