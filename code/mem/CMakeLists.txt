cmake_minimum_required(VERSION 3.8)
add_executable(mmapcopy mmapcopy.c)
target_link_libraries(mmapcopy LINK_PUBLIC csapp)
