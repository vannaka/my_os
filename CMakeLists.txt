cmake_minimum_required( VERSION 3.21 )

# Set baremetal x86 toolchain
set( CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/toolchains/i686_elf.cmake )

set( CMAKE_SYSTEM_NAME "Generic" )
set( CMAKE_SYSTEM_VERSION 1.0 )

project("MyOS" LANGUAGES C ASM)

set(SYS_ROOT "${CMAKE_CURRENT_LIST_DIR}/sysroot")
set(SYS_INC_DIR "${SYS_ROOT}/usr/include")
set(SYS_LIB_DIR : "${SYS_ROOT}/usr/lib")
set(SYS_BOOT_DIR : "${SYS_ROOT}/boot")

# Use C 11
set(CMAKE_C_STANDARD 11)

# Set install dir to sysroot folder
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}/sysroot")


# Add global compile/link flags
# TODO: These are gcc specific, so handle as such.
add_compile_options(
    "-ffreestanding"
    # "-fdump-tree-all-graph" # Dump AST as .dot file. View with graphviz
    "-isystem\"${SYS_INC_DIR}\""
    "--sysroot=${SYS_ROOT}"
    )
    
add_link_options(
    "-ffreestanding"
    "-nostdlib"
    "-lgcc" # Link libgcc to everything
    "--sysroot=${SYS_ROOT}"
    )
    
# Add sub-projects
add_subdirectory(lib/libc)
add_subdirectory(src) # Must be added after libc.