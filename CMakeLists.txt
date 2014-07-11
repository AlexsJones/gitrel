cmake_minimum_required(VERSION 2.8)
set(CMAKE_INSTALL_PREFIX /usr/local)
project(gitrel)
find_library(GITLIB NAMES libgit2 git2 HINTS "${CMAKE_PREFIX_PATH}/lib")

file(GLOB SRC "src/*.c")
add_executable(gitrel ${SRC})
target_link_libraries(gitrel git2 jnxcs.a)
install(TARGETS gitrel DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
