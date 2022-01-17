target_sources(kernel.elf
    PRIVATE
        "kernel.c"
    )

add_subdirectory(interrupts)
