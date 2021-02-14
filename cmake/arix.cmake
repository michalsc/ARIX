
function(project_type TYPE)
    set(PROJECT_TYPE ${TYPE} PARENT_SCOPE)
    set(PROJECT_CONFIG ${PROJECT_NAME}.json PARENT_SCOPE)
    set(PROJECT_CONFIG ${PROJECT_NAME}.json)
    
    get_verstring_date(PROJECT_VERSTRING)
    add_definitions(-DVERSION_STRING_DATE="${PROJECT_VERSTRING}")

    if(${TYPE} STREQUAL "library")
        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/__base_file.c
            COMMAND ${SFDC} basefile ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_CONFIG} > ${CMAKE_CURRENT_BINARY_DIR}/__base_file.c
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_CONFIG}
            VERBATIM
        )

        
    endif()
    set(GLOBAL_SYMBOLS "LibBase" PARENT_SCOPE)
    set(LOCAL_SYMBOLS "*" PARENT_SCOPE)
endfunction(project_type)

function(build_headers)
    add_custom_target(${PROJECT_NAME}.includes ALL
                      DEPENDS ${CMAKE_BINARY_DIR}/sys_include/proto/${PROJECT_NAME}.h
                      DEPENDS ${CMAKE_BINARY_DIR}/sys_include/inline/${PROJECT_NAME}.h
                      DEPENDS ${CMAKE_BINARY_DIR}/sys_include/defines/${PROJECT_NAME}.h
                      DEPENDS ${CMAKE_BINARY_DIR}/sys_include/clib/${PROJECT_NAME}_protos.h
                      )

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/sys_include/proto/${PROJECT_NAME}.h
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/sys_include/proto/
        COMMAND ${SFDC} header_proto ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_CONFIG} > ${CMAKE_BINARY_DIR}/sys_include/proto/${PROJECT_NAME}.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_CONFIG}
        VERBATIM
    )

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/sys_include/clib/${PROJECT_NAME}_protos.h
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/sys_include/clib/
        COMMAND ${SFDC} header_clib ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_CONFIG} > ${CMAKE_BINARY_DIR}/sys_include/clib/${PROJECT_NAME}_protos.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_CONFIG}
        VERBATIM
    )

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/sys_include/inline/${PROJECT_NAME}.h
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/sys_include/inline/
        COMMAND ${SFDC} header_inline ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_CONFIG} > ${CMAKE_BINARY_DIR}/sys_include/inline/${PROJECT_NAME}.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_CONFIG}
        VERBATIM
    )

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/sys_include/defines/${PROJECT_NAME}.h
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/sys_include/defines/
        COMMAND ${SFDC} header_defines ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_CONFIG} > ${CMAKE_BINARY_DIR}/sys_include/defines/${PROJECT_NAME}.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_CONFIG}
        VERBATIM
    )

endfunction(build_headers)

function(arix_finish TARGET)
    if (NOT ${CMAKE_HOST_SYSTEM_NAME} STREQUAL ${CMAKE_SYSTEM_NAME})
        include_directories(${CMAKE_BINARY_DIR}/sys_include)
    endif()
    if(${PROJECT_TYPE} STREQUAL "library")
        build_headers()
        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/version.script
            COMMAND ${CMAKE_COMMAND} -E echo "{ global: ${GLOBAL_SYMBOLS}; local: ${LOCAL_SYMBOLS}; };" > ${CMAKE_CURRENT_BINARY_DIR}/version.script
            VERBATIM
        )
        target_sources(${TARGET} PRIVATE
            ${CMAKE_CURRENT_BINARY_DIR}/__base_file
            ${CMAKE_CURRENT_BINARY_DIR}/version.script
        )
        add_dependencies(${TARGET}
            ${PROJECT_NAME}.includes
        )
    endif()
endfunction(arix_finish)

function(import_library LIB LIBNAME)
    add_library(${LIB} INTERFACE IMPORTED)
    set_property(TARGET ${LIB} PROPERTY IMPORTED_LIBNAME "${LIBNAME}")
endfunction(import_library)
