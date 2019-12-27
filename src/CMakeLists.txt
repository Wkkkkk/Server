SET(MODULE_DIRS
        core
        message
        driver
        test
        )

FOREACH (MODULE_DIR ${MODULE_DIRS})
    include_directories(${MODULE_DIR})
ENDFOREACH (MODULE_DIR)

# for backward include
FOREACH (MODULE_DIR ${MODULE_DIRS})
    add_subdirectory(${MODULE_DIR})
ENDFOREACH (MODULE_DIR)