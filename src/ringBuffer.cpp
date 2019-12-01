//
// Created by yxtwl on 30.11.19.
//
#include <sys/uio.h>
#include <cassert>
#include <sys/socket.h>

#include "ringBuffer.h"


ringBuffer::ringBuffer() {
    assert(readableBytes() == 0);
}

ssize_t ringBuffer::readFromFd(int fd) {

//    struct iovec iov[1];
//    iov[0].iov_base=&buf_.front();
//    iov[0].iov_len=initSize;
//    const ssize_t n=readv(fd,iov,1);

    char buff[initSize]="";
    const ssize_t n=recv(fd, buff, sizeof buff, 0);

    if(n>0){
        int i=0;
        while(true){
            if(buff[i]=='\n') break;  //以换行为结束位
            else buf_.push(buff[i]);
            i++;
        }
    }
    return n;
}

std::string ringBuffer::readBuffer() {

    std::string temp;
    while(readableBytes()){
        temp+=buf_.front();
        buf_.pop();
    }
    return temp;
}
