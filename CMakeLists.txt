cmake_minimum_required(VERSION 3.21)

set(MYOS_SYS_ROOT "${CMAKE_CURRENT_LIST_DIR}/sysroot")
set(MYOS_SYS_INC_DIR "${MYOS_SYS_ROOT}/usr/include")
set(MYOS_SYS_LIB_DIR : "${MYOS_SYS_ROOT}/usr/lib")
set(MYOS_BOOT_DIR : "${MYOS_SYS_ROOT}/boot")

set(MYOS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(MYOS_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

# set(MYOS_ARCH "x86" CACHE STRING "Target architecture for MYOS. Options: [x86]")
set(MYOS_ARCH "x86")

configure_file(
    ${MYOS_SOURCE_DIR}/tools/toolchain/CMake/gcc_toolchain.cmake.in
    ${MYOS_BUILD_DIR}/gcc_toolchain.cmake
    @ONLY
    )
set(CMAKE_TOOLCHAIN_FILE ${MYOS_BUILD_DIR}/gcc_toolchain.cmake)

set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_VERSION 1.0 )

project("MyOS" LANGUAGES C ASM)

set(CMAKE_C_STANDARD 23)

# Set install dir to sysroot folder
set(CMAKE_INSTALL_PREFIX ${MYOS_SYS_ROOT})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    
# Add sub-projects
add_subdirectory(lib/libc)
add_subdirectory(src) # Must be added after libc.