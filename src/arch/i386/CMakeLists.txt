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

# Add include dir to all targets in this list file 
#   and any descendents added with add_subdirectory
# include_directories(
#     "${CMAKE_CURRENT_LIST_DIR}/include"
#     )

add_subdirectory(boot)
add_subdirectory(interrupts)
add_subdirectory(memory)
add_subdirectory(tty)
add_subdirectory(gcc)