//
// Created by yxtwl on 30.11.19.
//
#include <sys/uio.h>
#include <cassert>
#include<boost/implicit_cast.hpp>

#include "ringBuffer.h"


ringBuffer::ringBuffer(size_t initialSize):
                buf_(initialSize),
                readIndex_(0),
                writeIndex_(1){

    assert(initialSize>=2);
    assert(readableBytes() == 0);
    assert(writableBytes()==initialSize-2);
}

ssize_t ringBuffer::readFd(int fd) {

    struct iovec vec[2];
    const size_t writeBytes = writableBytes();

    vec[0].iov_base = begin()+writeIndex_;
    vec[0].iov_len = writeBytes;

    const ssize_t n=readv(fd,vec,1);

    if(n<0){
        perror("read false");
    }
    else if (boost::implicit_cast<size_t>(n) <= writeBytes) {  //隐式转换　ssize_t->size_t,当空闲write空间足够时
        writeIndex_ += n;
    }
    else {
        writeIndex_ = buf_.size();    //当剩余空间不够时 加满
    }

    return n;
}
