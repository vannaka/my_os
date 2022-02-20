set(CMN_SRCS
    "printf.c"
    "putchar.c"
    "puts.c"
    )

target_sources(libc
    PRIVATE
        ${CMN_SRCS}
    )

target_sources(libk
    PRIVATE
        ${CMN_SRCS}
    )

target_include_directories(libk 
    PRIVATE
        "${CMAKE_SOURCE_DIR}/src/include" # TODO: Is this the best way of adding the kernel include dir?
    )
