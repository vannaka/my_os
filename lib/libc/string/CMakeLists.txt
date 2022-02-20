set(CMN_SRCS
    "memcmp.c"
    "memcpy.c"
    "memmove.c"
    "memset.c"
    "strlen.c"
    )

target_sources(libc
    PRIVATE
        ${CMN_SRCS}
    )

target_sources(libk
    PRIVATE
        ${CMN_SRCS}
    )
