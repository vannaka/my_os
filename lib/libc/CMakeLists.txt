cmake_minimum_required(VERSION 3.21)

project(libc LANGUAGES C)

set( SRCS
    "${CMAKE_CURRENT_LIST_DIR}/assert/assert.c"
    "${CMAKE_CURRENT_LIST_DIR}/ctype/toupper.c"
    "${CMAKE_CURRENT_LIST_DIR}/stdio/printf.c"
    "${CMAKE_CURRENT_LIST_DIR}/stdio/putchar.c"
    "${CMAKE_CURRENT_LIST_DIR}/stdio/puts.c"
    "${CMAKE_CURRENT_LIST_DIR}/stdlib/abort.c"
    "${CMAKE_CURRENT_LIST_DIR}/stdlib/itoa.c"
    "${CMAKE_CURRENT_LIST_DIR}/stdlib/utoa.c"
    "${CMAKE_CURRENT_LIST_DIR}/string/memcmp.c"
    "${CMAKE_CURRENT_LIST_DIR}/string/memcpy.c"
    "${CMAKE_CURRENT_LIST_DIR}/string/memmove.c"
    "${CMAKE_CURRENT_LIST_DIR}/string/memset.c"
    "${CMAKE_CURRENT_LIST_DIR}/string/strlen.c"
    )

# Userspace libc
add_library(libc STATIC ${SRCS})

# Kernelspace libc
add_library(libk STATIC ${SRCS})

target_compile_definitions(
    libc PRIVATE 
    -D__is_libc
    )

target_compile_definitions(
    libk PRIVATE 
    -D__is_libk
    )

target_include_directories(
    libc PUBLIC 
    "${CMAKE_CURRENT_LIST_DIR}/include"
    )

target_include_directories(
    libk 
    PUBLIC 
        "${CMAKE_CURRENT_LIST_DIR}/include"
    )

# target_sources( 
#     libc
#     PRIVATE
#         "${CMAKE_CURRENT_LIST_DIR}/assert/assert.c"
#         "${CMAKE_CURRENT_LIST_DIR}/ctype/toupper.c"
#         "${CMAKE_CURRENT_LIST_DIR}/stdio/printf.c"
#         "${CMAKE_CURRENT_LIST_DIR}/stdio/putchar.c"
#         "${CMAKE_CURRENT_LIST_DIR}/stdio/puts.c"
#         "${CMAKE_CURRENT_LIST_DIR}/stdlib/abort.c"
#         "${CMAKE_CURRENT_LIST_DIR}/stdlib/itoa.c"
#         "${CMAKE_CURRENT_LIST_DIR}/stdlib/utoa.c"
#         "${CMAKE_CURRENT_LIST_DIR}/string/memcmp.c"
#         "${CMAKE_CURRENT_LIST_DIR}/string/memcpy.c"
#         "${CMAKE_CURRENT_LIST_DIR}/string/memmove.c"
#         "${CMAKE_CURRENT_LIST_DIR}/string/memset.c"
#         "${CMAKE_CURRENT_LIST_DIR}/string/strlen.c"
#     )
