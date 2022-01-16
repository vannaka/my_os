set(CMN_SRCS
    "abort.c"
    "itoa.c"
    "utoa.c"
    )

target_sources(libc
    PRIVATE
        ${CMN_SRCS}
    )

target_sources(libk
    PRIVATE
        ${CMN_SRCS}
    )
