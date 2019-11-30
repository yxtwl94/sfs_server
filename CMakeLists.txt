cmake_minimum_required(VERSION 3.15)
project(sfs_server)

set(CMAKE_CXX_STANDARD 14)
find_package(Threads REQUIRED)

include_directories(.)

add_library(${PROJECT_NAME}
        src/client.cpp test/client_test.cpp)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
link_libraries(${PROJECT_NAME})

add_executable(client_test test/client_test.cpp)