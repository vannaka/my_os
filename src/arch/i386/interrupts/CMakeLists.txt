# Since GCC doesn’t preserve SSE, MMX nor x87 states, 
#   the GCC option -mgeneral-regs-only should be used 
#   to compile interrupt and exception handlers.
add_library(intr_hndlrs STATIC
    "handlers/intr_handlers.c"
    )

target_compile_options(intr_hndlrs
    PRIVATE
        "-mgeneral-regs-only"
    )

# Add kernel includes to inetrrupt handler lib
# TODO: This feels hacky
get_target_property(KRNL_INC_DIRS kernel INCLUDE_DIRECTORIES)

target_include_directories(intr_hndlrs
    PRIVATE
        KRNL_INC_DIRS
    )

target_link_libraries(intr_hndlrs
    PRIVATE
        libk
    )

target_sources(kernel
    PRIVATE
        "idt/idt_asm.S"
        "idt/idt.c"
        "controller/intc_arch.c"
        "controller/pic/pic.c"
    )

target_link_libraries(kernel
    intr_hndlrs
    )