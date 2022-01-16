target_sources(kernel
    PRIVATE
        "kernel.c"
    )

add_subdirectory(interrupts)
