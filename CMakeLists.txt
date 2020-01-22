cmake_minimum_required(VERSION 3.10)
project(sfs_server)

set(CMAKE_CXX_STANDARD 14)
find_package(Threads REQUIRED)

include_directories(.)

add_library(${PROJECT_NAME}
        src/Server.cpp
        src/RingBuffer.cpp
        src/Channel.cpp
        src/EventLoop.cpp
        src/Poller.cpp
        src/EventLoopThread.cpp
        src/EventLoopThreadPool.cpp
        )

link_libraries(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} Threads::Threads)

add_executable(server_node
        test/server_test.cpp
        )
