find_package(Check REQUIRED)

set(TESTS
    "test_vector"
    "test_mesh"
    "test_files"
    "test_vec_math"
    "test_linked_list"
    "test_enum_hashtable"
)

foreach(T ${TESTS})
    add_executable(${T} ${T}.c)
    add_dependencies(${T} utils)
    target_compile_options(${T}
        PUBLIC
            $<$<CONFIG:DEBUG>:-Wall -Wpedantic -Wextra -std=c17>)

    target_include_directories(${T}
        PUBLIC
            ${CHECK_INCLUDE_DIRS}
            ${PROJECT_SOURCE_DIR}/src/utils
    )
    target_link_libraries(${T} $<TARGET_FILE:utils> ${CHECK_LIBRARIES})

    add_test(${T} ${T})
endforeach()

file(COPY mock_assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
