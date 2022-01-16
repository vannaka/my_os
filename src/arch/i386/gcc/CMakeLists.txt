execute_process(
    COMMAND echo 
        ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_<CONFIG>} -print-file-name=crtbegin.o 
    OUTPUT_VARIABLE CRTBEGIN_PATH
    )

execute_process(
    COMMAND echo 
        ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_<CONFIG>} -print-file-name=crtend.o
    OUTPUT_VARIABLE CRTEND_PATH
    )

target_sources(kernel
    "crti.S"
    ${CRTBEGIN_PATH}
    ${CRTEND_PATH}
    "crtn.S"
    )