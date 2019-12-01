//
// Created by yxt on 30.11.19.
//


#ifndef SFS_SERVER_RINGBUFFER_H
#define SFS_SERVER_RINGBUFFER_H


#include <queue>
#include <iostream>
#include <algorithm>

class ringBuffer {

public:
    static const size_t initSize = 1024;
    explicit ringBuffer();

    size_t readableBytes() const {
        return buf_.size();
    };

    ssize_t readFromFd(int fd);
    std::string readBuffer();

private:
    std::queue<char> buf_;
};

#endif //SFS_SERVER_RINGBUFFER_H
