add_library(utils "")
target_sources(utils
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/vector.c
)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_options(utils
    PUBLIC
        $<$<CONFIG:RELEASE>:-Wall -Werror -Wpedantic -std=c11 -O3>
        $<$<CONFIG:DEBUG>:-Wall -Werror -Wpedantic -std=c11 -g>)
