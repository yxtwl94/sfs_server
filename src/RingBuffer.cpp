//
// Created by yxt on 30.11.19.
//
#include <sys/uio.h>
#include <cassert>
#include <boost/implicit_cast.hpp>
#include "RingBuffer.h"


RingBuffer::RingBuffer(size_t initialSize):
                            buf_(initialSize),
                            readIndex_(0),
                            writeIndex_(0) {

    assert(initialSize>=2);
    assert(readableBytes() == 0);
    assert(writableBytesR()==initialSize-1);
    assert(writableBytesL()==1);
}

ssize_t RingBuffer::readFromFd(int fd) {

    struct iovec iov[2];
    const size_t writeBytesR = writableBytesR();
    const size_t writeBytesL = writableBytesL();

    iov[0].iov_base =begin()+writeIndex_+1;
    iov[0].iov_len = writeBytesR;

    iov[1].iov_base =begin();  //if writableBytesR full it will try fill data into begin of the buffer
    iov[1].iov_len = writeBytesL;

    if(writeBytesR==0 && writeBytesL==0){
        fprintf(stderr,"buffer full error\n");
    }

    const ssize_t n=readv(fd,iov,2);

    //printf("right %zu,left %zu\n",writeBytesR,writeBytesL);

    if(n<0){
        perror("read error\n");
    }
    else if (boost::implicit_cast<size_t>(n) <= writeBytesR+writeBytesL) {  //隐式转换　ssize_t->size_t,当空闲write空间足够时
        if(writeIndex_+n<=buf_.size()-1)
            writeIndex_+=n;
        else
            writeIndex_=writeIndex_+n-buf_.size();
    }
    else {                                      //当剩余空间不够时 加满
        if(readIndex_==0)
            writeIndex_=buf_.size()-1;
        else
            writeIndex_ = readIndex_-1;
    }

    return n;
}

std::string RingBuffer::getBuffer() {

    std::string temp;
    for(size_t i=readIndex_+1;;++i){
        if(i==buf_.size()) i=0;  //加完1之后越界返回
        if(i==writeIndex_+1) break;

        if(buf_[i]=='\n'){
            buf_[i]= '\0';
            readIndex_=i;
            return temp;
        }
        else{
            temp+=buf_[i];
            buf_[i]='\0';
        }
    }
}
