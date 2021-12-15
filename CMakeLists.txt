cmake_minimum_required( VERSION 3.21 )

# Set baremetal x86 toolchain
set( CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/toolchains/i686_elf.cmake )

set( CMAKE_SYSTEM_NAME "Generic" )
set( CMAKE_SYSTEM_VERSION 1.0 )

project("MyOS" LANGUAGES C ASM)