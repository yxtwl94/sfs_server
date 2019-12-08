//
// Created by yxtwl94 on 03.12.19.
//

#include <sys/eventfd.h>
#include <cassert>

#include "Channel.h"

nio::Channel::Channel(EventLoop* loop):
                            loop_(loop),
                            fd_(0),
                            event_(0),
                            revent_(0),
                            lastEvent_(0){

    fd_=eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    assert(fd_>0);
}

void nio::Channel::handelEvents() {
    event_=0;

    //文件被挂断,!文件可读
    if((revent_ & EPOLLHUP) && !(revent_ & EPOLLIN)){
        return;
    }
    //文件上发上了一个错误
    if (revent_ & EPOLLERR) {
        return;
    }
    //文件可读,文件有紧急数据可读,对端关闭连接或者shutdown写入半连接
    if (revent_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
        if(readHandler_) readHandler_();
    }
    //文件可写
    if (revent_ & EPOLLOUT) {
        if(writeHandler_) writeHandler_();
    }

}


void nio::Channel::setReadHandler(CallBack &&readHandler) { readHandler_ = readHandler; }

void nio::Channel::setWriteHandler(CallBack &&writeHandler) { writeHandler_=writeHandler; }


nio::Channel::~Channel() = default;
