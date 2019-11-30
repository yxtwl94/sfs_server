//
// Created by yxt on 30.11.19.
//


#ifndef SFS_SERVER_RINGBUFFER_H
#define SFS_SERVER_RINGBUFFER_H


#include <vector>
#include <iostream>
#include <algorithm>

class ringBuffer {

public:
    static const size_t initSize = 1026;  //1024+2
    explicit ringBuffer(size_t initialSize = initSize);

    size_t readableBytes() const {
        if(writeIndex_>readIndex_)
            return writeIndex_-readIndex_-1;
        else
            return buf_.size()-(readIndex_-writeIndex_+1);
    };

    size_t writableBytes() const {
        return buf_.size()-readableBytes()-2;
    };

    ssize_t readFd(int fd);
private:

    std::vector<char> buf_;
    size_t readIndex_;  //无符号　因为均大于０
    size_t writeIndex_;

    char* begin()
    { return &*buf_.begin(); }

    const char* begin() const
    { return &*buf_.begin(); }
};


#endif //SFS_SERVER_RINGBUFFER_H
