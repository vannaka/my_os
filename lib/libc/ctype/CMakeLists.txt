set(CMN_SRCS
    "toupper.c"
    )

target_sources(libc
    PRIVATE
        ${CMN_SRCS}
    )

target_sources(libk
    PRIVATE
        ${CMN_SRCS}
    )
