# CMake toolchain file for the generic i686 elf gcc toolchain

set(CMAKE_SYSTEM_NAME           Generic)
set(CMAKE_SYSTEM_PROCESSOR      "x86")
set(CMAKE_SYSTEM_VERSION        "1.0")

# Without that flag CMake is not able to pass test compilation check
set(CMAKE_TRY_COMPILE_TARGET_TYPE   STATIC_LIBRARY)

# This should be set on the command line w/ -D flag
set( MY_OS_TOOLCHAIN_PATH "" CACHE PATH "" )

if(EXISTS "${MY_OS_TOOLCHAIN_PATH}" AND IS_DIRECTORY "${MY_OS_TOOLCHAIN_PATH}")
    set( toolchain_dir "${MY_OS_TOOLCHAIN_PATH}/" )
elseif( "${MY_OS_TOOLCHAIN_PATH}" STREQUAL "" )
    # Hopefully the toolchain is on the path
    set( toolchain_dir "" )
else()
    message(FATAL_ERROR "Path given in MY_OS_TOOLCHAIN_PATH does not exist! - ${MY_OS_TOOLCHAIN_PATH}")
endif()

set(CMAKE_AR                        ${toolchain_dir}i686-elf-ar${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_ASM_COMPILER              ${toolchain_dir}i686-elf-gcc${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_C_COMPILER                ${toolchain_dir}i686-elf-gcc${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_CXX_COMPILER              ${toolchain_dir}i686-elf-g++${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_LINKER                    ${toolchain_dir}i686-elf-ld${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_OBJCOPY                   ${toolchain_dir}i686-elf-objcopy${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_RANLIB                    ${toolchain_dir}i686-elf-ranlib${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_SIZE                      ${toolchain_dir}i686-elf-size${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_STRIP                     ${toolchain_dir}i686-elf-strip${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")

set(CMAKE_C_FLAGS                   "-ffreestanding" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS                 "${CMAKE_C_FLAGS} -fno-exceptions" CACHE INTERNAL "")

set(CMAKE_C_FLAGS_DEBUG             "-Os -g" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE           "-Os -DNDEBUG" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_C_FLAGS_DEBUG}" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_C_FLAGS_RELEASE}" CACHE INTERNAL "")

set(CMAKE_EXE_LINKER_FLAGS          "-nostdlib -nostdinc -lgcc" )

# Find crtbegin.o and crtend.o
execute_process(
    COMMAND 
        ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} -print-file-name=crtbegin.o
    OUTPUT_VARIABLE CRTBEGIN_OBJ
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
execute_process(
    COMMAND 
        ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} -print-file-name=crtend.o
    OUTPUT_VARIABLE CRTEND_OBJ
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

# Object 'crtbegin.o' will be linked before all other objects.
# Object 'crtend.o' will be linked after all other objects.
SET(CMAKE_C_LINK_EXECUTABLE 
    "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> ${CRTBEGIN_OBJ} <OBJECTS> ${CRTEND_OBJ} -o <TARGET> <LINK_LIBRARIES>"
    )

# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)