cmake_minimum_required(VERSION 3.21)

project(libc LANGUAGES C)

#-----------------------------------------------------------
# Standard C Lib targets
#-----------------------------------------------------------
# Userspace libc
add_library(libc STATIC "")

# Userspace not supported yet so don't build it
set_target_properties(libc
    PROPERTIES    
        EXCLUDE_FROM_ALL TRUE
    )

# Kernelspace libc
add_library(libk STATIC "")

target_compile_definitions(libc
    PRIVATE
        -D__is_libc
    )

target_compile_definitions(libk
    PRIVATE
        -D__is_libk
    )

#-----------------------------------------------------------
# Common sources between kernel and userspace
#-----------------------------------------------------------
set( CMN_SRCS
    assert/assert.c
    ctype/toupper.c
    stdio/printf.c
    stdio/putchar.c
    stdio/puts.c
    stdlib/abort.c
    stdlib/itoa.c
    stdlib/utoa.c
    string/memcmp.c
    string/memcpy.c
    string/memmove.c
    string/memset.c
    string/strlen.c

    # Inlclude files
    include/sys/cdefs.h
    include/assert.h
    include/ctype.h
    include/feature.h
    include/stdio.h
    include/stdlib.h
    include/string.h
    )

set( CMN_INC_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/include
    )

#-----------------------------------------------------------
# Add sources to libs
#-----------------------------------------------------------
target_sources(libc
    PRIVATE
        ${CMN_SRCS}
    )

target_sources(libk
    PRIVATE
        ${CMN_SRCS}
    )

#-----------------------------------------------------------
# Add include dirs to libs
#-----------------------------------------------------------
target_include_directories(libc
    PUBLIC
        ${CMN_INC_DIRS}
    )

target_include_directories(libk
    PUBLIC
        ${CMN_INC_DIRS}
    PRIVATE
        # Need kernel headers for putchar()
        "${CMAKE_SOURCE_DIR}/src/include" # TODO: Is this the best way of adding the kernel include dir?
    )

#-----------------------------------------------------------
# Install std c lib and headers
#-----------------------------------------------------------
install(TARGETS libk LIBRARY DESTINATION "usr/lib")
install(DIRECTORY "include" DESTINATION "usr")