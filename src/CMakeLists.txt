add_subdirectory(utils)
add_subdirectory(engine)

add_executable(LonelyTennis "")
add_dependencies(LonelyTennis utils engine)
target_sources(LonelyTennis
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/game.c
        ${CMAKE_CURRENT_LIST_DIR}/wall.c
        ${CMAKE_CURRENT_LIST_DIR}/ball.c
)
target_include_directories(LonelyTennis PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_compile_options(LonelyTennis
    PUBLIC
        $<$<CONFIG:RELEASE>:-Wall -Wpedantic -Wextra -Werror -std=c17 -O3 -ffast-math>
        $<$<CONFIG:DEBUG>:-Wall -Wpedantic -Wextra -std=c17 -ffast-math>)

target_link_libraries(LonelyTennis $<TARGET_FILE:engine> $<TARGET_FILE:utils> m GL GLEW SDL2 SDL2_image)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
