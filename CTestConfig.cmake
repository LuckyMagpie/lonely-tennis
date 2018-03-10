set(MEMORYCHECK_SUPPRESSIONS_FILE "${CMAKE_SOURCE_DIR}/valgrind.supp" CACHE FILEPATH "Path to valgrind suppressions")
set(MEMORYCHECK_COMMAND_OPTIONS "--read-var-info=yes --leak-check=full --show-leak-kinds=all")
