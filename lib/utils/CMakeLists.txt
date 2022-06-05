cmake_minimum_required(VERSION 3.21)

project("utils" LANGUAGES C)

# Create utils library target
add_library(utils INTERFACE )

# Add include directories to utils target
target_include_directories(utils 
    INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_sources(utils
    PRIVATE    
        bit_array.h
    )
