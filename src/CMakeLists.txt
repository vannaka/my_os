cmake_minimum_required(VERSION 3.21)

project("kernel" LANGUAGES C ASM)

add_executable(kernel.elf "")

# Link with required libraries
target_link_libraries(kernel.elf
    libk
    )

# Add include dir to kernel target
target_include_directories(kernel.elf
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/include"
    )

# Add include dir to all targets in this list file 
#   and any descendents added with add_subdirectory
# include_directories(
#     "${CMAKE_CURRENT_LIST_DIR}/include"
#     )

target_compile_definitions(kernel.elf
    PRIVATE    
        "__is_kernel"
    )

add_subdirectory(kernel)

# Add appropriate arch directory
if( CMAKE_SYSTEM_PROCESSOR MATCHES "x86" )
    add_subdirectory(arch/i386)
else()
    message(FATAL_ERROR "Arch ${CMAKE_SYSTEM_PROCESSOR} not supported")
endif()