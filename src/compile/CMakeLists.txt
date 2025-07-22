
target_sources(
    ${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/compile-ctx.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/compile-pass.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/compile.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/pass1.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/pass2.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/print.cpp"
)
