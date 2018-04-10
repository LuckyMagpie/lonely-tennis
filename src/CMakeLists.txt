add_subdirectory(utils)

add_library(game "")
target_sources(game
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/game.c
        ${CMAKE_CURRENT_LIST_DIR}/graphics.c
        ${CMAKE_CURRENT_LIST_DIR}/render.c
        ${CMAKE_CURRENT_LIST_DIR}/world.c
        ${CMAKE_CURRENT_LIST_DIR}/wall.c
        ${CMAKE_CURRENT_LIST_DIR}/ball.c
)
target_include_directories(game PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/utils)
target_compile_options(game
    PUBLIC
        $<$<CONFIG:RELEASE>:-Wall -Wpedantic -Wextra -Werror -std=c17 -O3 -ffast-math>
        $<$<CONFIG:DEBUG>:-Wall -Wpedantic -Wextra -std=c17 -ffast-math>)
target_link_libraries(game utils)

add_executable(LonelyTennis main.c)
target_link_libraries(LonelyTennis $<TARGET_FILE:game> m GL GLEW SDL2)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
