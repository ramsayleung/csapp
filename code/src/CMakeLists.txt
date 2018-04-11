cmake_minimum_required(VERSION 3.8)
add_library (csapp csapp.c)
target_include_directories (csapp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
