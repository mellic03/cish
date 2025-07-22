cmake_minimum_required(VERSION 3.31)
SET(CMAKE_CXX_FLAGS -std=c++23)

project(cish)

include_directories("${CMAKE_SOURCE_DIR}/include/")

add_executable(
    ${PROJECT_NAME}
)

target_sources(
    ${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/assembler")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/cish")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/compile")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/lexer")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/parser")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/parser_gen")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/vm")

