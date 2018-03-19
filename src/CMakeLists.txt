add_subdirectory(utils)

add_library(game "")
target_sources(game
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/game.c
        ${CMAKE_CURRENT_LIST_DIR}/graphics.c
        ${CMAKE_CURRENT_LIST_DIR}/render.c
        ${CMAKE_CURRENT_LIST_DIR}/world.c
)
target_include_directories(game PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/utils)
target_compile_options(game
    PUBLIC
        $<$<CONFIG:RELEASE>:-Wall -Wpedantic -Wextra -Werror -std=c11 -O3>
        $<$<CONFIG:DEBUG>:-Wall -Wpedantic -Wextra -std=c11>)
target_link_libraries(game utils)

add_executable(LonelyTennis main.c)
target_link_libraries(LonelyTennis game)
