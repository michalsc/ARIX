if(__ARIX_HELPER_CMAKE_INCLUDED)
  return()
endif()
set(__ARIX_HELPER_CMAKE_INCLUDED TRUE)

# Build version string in Amiga-like manner: $VER: project_name version (build_date) git:git_hash"
function(get_verstring OUTPUT)
    find_package(Git)
    string(TIMESTAMP PROJECT_DATE "(%d.%m.%Y)")
    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE GIT_HASH OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${GIT_EXECUTABLE} status --porcelain WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE GIT_PORCELAIN OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (NOT GIT_PORCELAIN STREQUAL "")
        set(GIT_PORCELAIN ",dirty")
    endif()
    set(${OUTPUT} "$VER: ${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_DATE} git:${GIT_HASH}${GIT_PORCELAIN}" PARENT_SCOPE)
endfunction(get_verstring)

# Build version string in Amiga-like manner: $VER: project_name version (build_date) git:git_hash"
function(get_verstring_date OUTPUT)
    find_package(Git)
    string(TIMESTAMP PROJECT_DATE "(%d.%m.%Y)")
    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE GIT_HASH OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${GIT_EXECUTABLE} status --porcelain WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE GIT_PORCELAIN OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (NOT GIT_PORCELAIN STREQUAL "")
        set(GIT_PORCELAIN ",dirty")
    endif()
    set(${OUTPUT} "${PROJECT_DATE} git:${GIT_HASH}${GIT_PORCELAIN}" PARENT_SCOPE)
endfunction(get_verstring_date)
