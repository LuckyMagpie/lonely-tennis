find_package(Check REQUIRED)

set(TESTS
    "test_vector"
)


foreach(T ${TESTS})
    add_executable(${T} ${T}.c)

    target_include_directories(${T}
        PUBLIC
            ${CHECK_INCLUDE_DIRS}
            ${PROJECT_SOURCE_DIR}/src/utils
    )
    target_link_libraries(${T} utils ${CHECK_LIBRARIES})

    add_test(${T} ${T})
endforeach()
