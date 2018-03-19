add_library(utils "")
target_sources(utils
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/vector.c
        ${CMAKE_CURRENT_LIST_DIR}/mesh.c
)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_options(utils
    PUBLIC
        $<$<CONFIG:RELEASE>:-Wall -Wpedantic -Wextra -Werror -std=c11 -O3>
        $<$<CONFIG:DEBUG>:-Wall -Wpedantic -Wextra -std=c11>)
