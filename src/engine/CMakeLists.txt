add_library(engine "")
add_dependencies(engine utils)
target_sources(engine
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/graphics.c
        ${CMAKE_CURRENT_LIST_DIR}/render.c
        ${CMAKE_CURRENT_LIST_DIR}/world.c
        ${CMAKE_CURRENT_LIST_DIR}/colission.c
        ${CMAKE_CURRENT_LIST_DIR}/context.c
)
target_include_directories(engine PUBLIC ${PROJECT_SOURCE_DIR}/src
)
target_compile_options(engine
    PUBLIC
        $<$<CONFIG:RELEASE>:-Wall -Wpedantic -Wextra -Werror -std=c17 -O3 -ffast-math>
        $<$<CONFIG:DEBUG>:-Wall -Wpedantic -Wextra -std=c17 -ffast-math>)

target_link_libraries(engine $<TARGET_FILE:utils>)
