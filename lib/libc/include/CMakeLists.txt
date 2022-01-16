set(LIBC_CMN_HEADERS
    "sys/cdefs.h"
    "assert.h"
    "ctype.h"
    "feature.h"
    "stdio.h"
    "stdlib.h"
    "string.h"
    )

target_sources(libc
    PUBLIC
        ${LIBC_CMN_HEADERS}
    )

target_sources(libk
    PUBLIC
        ${LIBC_CMN_HEADERS}
    )

target_include_directories(libc
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    )

target_include_directories(libk
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    )