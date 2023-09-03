target_link_options(kernel.elf
    PRIVATE
        "-T${CMAKE_CURRENT_LIST_DIR}/linker.ld"
    )

# Add dependency on the kernel target to the linker script
set_target_properties(kernel.elf
    PROPERTIES 
        LINK_DEPENDS 
            "${CMAKE_CURRENT_LIST_DIR}/linker.ld"
    )

target_include_directories(kernel.elf
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/include"
        "${CMAKE_CURRENT_LIST_DIR}/include/interrupts" # Needed for "interrupts_arch.h". TODO: Think about this more.
    )

target_sources(kernel.elf
    PRIVATE
        boot/boot.S
        interrupts/controller/intc_arch.c
        interrupts/controller/pic/pic.c
        interrupts/idt/excpt_hndlrs.c
        interrupts/idt/idt.c
        interrupts/idt/idt_asm.S
        interrupts/idt/vectors.S
        memory/gdt/gdt_asm.S
        memory/gdt/gdt.c
        memory/memory.c
        tty/tty.c
    )

#-----------------------------------------------------------
# Since GCC doesn’t preserve SSE, MMX nor x87 states,
#   the GCC option -mgeneral-regs-only should be used
#   to compile interrupt and exception handlers.
#-----------------------------------------------------------
set_source_files_properties( interrupts/handlers/intr_handlers.c
    TARGET_DIRECTORY
        kernel.elf
    PROPERTIES
        COMPILE_FLAGS -mgeneral-regs-only
    )

#-----------------------------------------------------------
# Arch + Toolchain specific sources
#-----------------------------------------------------------
if( CMAKE_C_COMPILER_ID MATCHES "GNU" )
    add_subdirectory(gcc)
else()
    message(FATAL_ERROR "Toolchain \"${CMAKE_C_COMPILER_ID}\" not supported!")
endif()