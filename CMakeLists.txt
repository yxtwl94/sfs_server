cmake_minimum_required(VERSION 3.15)
project(sfs_server)

set(CMAKE_CXX_STANDARD 14)
find_package(Threads REQUIRED)

include_directories(.)

add_library(${PROJECT_NAME}
            src/server.cpp)
link_libraries(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} Threads::Threads)

add_executable(test_node
        test/server_test.cpp
        )
