target_sources(kernel.elf
    PRIVATE
        "gdt/gdt_asm.S"
        "gdt/gdt.c"
        "memory.c"
    )