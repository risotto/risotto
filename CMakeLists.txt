cmake_minimum_required(VERSION 3.15)
project(risottov2)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE LIB_SRC RELATIVE ${CMAKE_SOURCE_DIR} "src/lib/*.c" "src/lib/*.cpp" "src/lib/*.h")
file(GLOB_RECURSE CMD_SRC RELATIVE ${CMAKE_SOURCE_DIR} "src/cmd/*.c" "src/cmd/*.cpp" "src/cmd/*.h")

# LIBRARY

add_library(risotto STATIC ${LIB_SRC})
target_include_directories (risotto PUBLIC ${CMAKE_SOURCE_DIR}/src)


add_executable(cmd ${CMD_SRC})
target_link_libraries(cmd risotto)
