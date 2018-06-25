find_package(Check REQUIRED)

set(TESTS
    "test_world"
    "test_colission"
    "test_executor"
)


foreach(T ${TESTS})
    add_executable(${T} ${T}.c)
    add_dependencies(${T} utils engine)
    target_compile_options(${T}
        PUBLIC
            $<$<CONFIG:DEBUG>:-Wall -Wpedantic -Wextra -std=c17>)

    target_include_directories(${T}
        PUBLIC
            ${CHECK_INCLUDE_DIRS}
            ${PROJECT_SOURCE_DIR}/src
    )
    target_link_libraries(${T} $<TARGET_FILE:engine> $<TARGET_FILE:utils> ${CHECK_LIBRARIES} m GL GLEW SDL2 SDL2_image)

    add_test(${T} ${T})
endforeach()
