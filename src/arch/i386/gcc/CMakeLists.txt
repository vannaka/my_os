# add_library(CRTI_OBJ OBJECT "crti.S")
# add_library(CRTN_OBJ OBJECT "crtn.S")

# add_dependencies(kernel.elf
#     CRTI_OBJ CRTN_OBJ
#     )

target_sources(kernel.elf
    PRIVATE
        "crti.S"
        "crtn.S"
    )