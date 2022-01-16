target_sources(kernel
    PRIVATE
        "gdt/gdt_asm.S"
        "gdt/gdt.c"
        "memory.c"
    )