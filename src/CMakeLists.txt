add_library(game "")
target_sources(game
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/game.c
        ${CMAKE_CURRENT_LIST_DIR}/graphics.c
        ${CMAKE_CURRENT_LIST_DIR}/render.c
        ${CMAKE_CURRENT_LIST_DIR}/world.c
)
target_include_directories(game PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_options(game
    PUBLIC
        $<$<CONFIG:RELEASE>:-Wall -Werror -Wpedantic -std=c11 -O3>
        $<$<CONFIG:DEBUG>:-Wall -Werror -Wpedantic -std=c11 -g>)
target_link_libraries(game GL GLEW SDL2)

add_executable(LonelyTennis main.c)
target_link_libraries(LonelyTennis game)
