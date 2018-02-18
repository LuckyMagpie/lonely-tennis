find_package(Check REQUIRED)

set(TESTS
    "test_world"
)


foreach(T ${TESTS})
    add_executable(${T} ${T}.c)

    target_include_directories(${T}
        PUBLIC
            ${CHECK_INCLUDE_DIRS}
            ${PROJECT_SOURCE_DIR}/src
    )
    target_link_libraries(${T} game ${CHECK_LIBRARIES})

    add_test(${T} ${T})
    add_test(${T}_valgrind valgrind
        --error-exitcode=0 --read-var-info=yes
        --leak-check=full --show-leak-kinds=all
        ./${T})
endforeach()
