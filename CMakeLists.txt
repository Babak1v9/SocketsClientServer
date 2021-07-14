cmake_minimum_required(VERSION 3.17)
project(vs)

set(CMAKE_CXX_STANDARD 20)

set(GCC_COVERAGE_COMPILE_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(server server.cpp)
add_executable(client client.cpp)
add_executable(serverSynchro serverSynchro.cpp)
add_executable(serverSynchroEncrypt serverSynchroEncrypt.cpp)