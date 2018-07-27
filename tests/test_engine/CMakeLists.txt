find_package(Check REQUIRED)

set(TESTS
    "test_world"
    "test_colission"
    "test_context"
)

foreach(T ${TESTS})
    add_executable(${T} ${T}.c ${PROJECT_SOURCE_DIR}/tests/cmp_helpers.c ${PROJECT_SOURCE_DIR}/tests/fixtures.c)
    add_dependencies(${T} utils engine)
    target_compile_options(${T}
        PUBLIC
            $<$<CONFIG:DEBUG>:-Wall -Wpedantic -Wextra -std=c17>)

    target_include_directories(${T}
        PUBLIC
            ${CHECK_INCLUDE_DIRS}
            ${PROJECT_SOURCE_DIR}/src
            ${PROJECT_SOURCE_DIR}/tests
    )
    target_link_libraries(${T} $<TARGET_FILE:engine> $<TARGET_FILE:utils> ${CHECK_LIBRARIES} m GL GLEW SDL2 SDL2_image)

    add_test(${T} ${T})
endforeach()

file(COPY mock_assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
