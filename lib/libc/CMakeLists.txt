cmake_minimum_required(VERSION 3.21)

project(libc LANGUAGES C)

# Userspace libc
add_library(libc STATIC "")

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

add_subdirectory(include)
add_subdirectory(assert)
add_subdirectory(ctype)
add_subdirectory(stdio)
add_subdirectory(stdlib)
add_subdirectory(string)

install(TARGETS libk LIBRARY DESTINATION "usr/lib")
install(DIRECTORY "include" DESTINATION "usr/include")