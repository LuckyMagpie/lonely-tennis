cmake_minimum_required (VERSION 3.0)
project ("LonelyTennis" C)
enable_testing()
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")


add_subdirectory(src)
add_subdirectory(test)
