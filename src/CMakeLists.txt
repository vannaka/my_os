cmake_minimum_required( VERSION 3.21 )

project("kernel" LANGUAGES C ASM)

add_executable(kernel 
    "kernel/kernel.c"
    "kernel/interrupts/intc.c"
    )

# Link with required libraries
target_link_libraries( kernel
    libc
    )
