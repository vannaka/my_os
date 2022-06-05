cmake_minimum_required(VERSION 3.21)

project("kernel" LANGUAGES C ASM)

add_executable(kernel.elf "")

# Link with required libraries
target_link_libraries(kernel.elf
    libk
    utils
    )

# Add include dir to kernel target
target_include_directories(kernel.elf
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/include"
    )

target_compile_definitions(kernel.elf
    PRIVATE    
        __is_kernel
    )

#-----------------------------------------------------------
# Kernel sources
#-----------------------------------------------------------
target_sources(kernel.elf
    PRIVATE
        kernel/kernel.c
        kernel/interrupts/intc.c
        kernel/memory/page_frame.c
    )

#-----------------------------------------------------------
# Arch sources
#-----------------------------------------------------------
if( CMAKE_SYSTEM_PROCESSOR MATCHES "x86" )
    add_subdirectory(arch/i386)
else()
    message(FATAL_ERROR "Target arch \"${CMAKE_SYSTEM_PROCESSOR}\" not supported!")
endif()