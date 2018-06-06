cmake_minimum_required (VERSION 3.0)
project ("LonelyTennis" C)

include (CTest)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

add_definitions(-DM_PI=3.14159265358979323846 -DM_PI_2=1.57079632679489661923 -DM_PI_4=0.785398163397448309616)

add_subdirectory(src)
add_subdirectory(tests)
