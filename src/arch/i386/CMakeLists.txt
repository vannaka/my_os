target_link_options(kernel
    PRIVATE
        "-T ${CMAKE_CURRENT_LIST_DIR}/linker.ld"
    )

# Add dependency on the kernel to the linker script
set_target_properties(kernel
    PROPERTIES 
        LINK_DEPENDS 
            "${CMAKE_CURRENT_LIST_DIR}/linker.ld"
    )

target_include_directories(kernel
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/include"
    )

add_subdirectory(boot)
add_subdirectory(interrupts)
add_subdirectory(memory)
