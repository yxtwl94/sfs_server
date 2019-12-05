//
// Created by yxtwl94 on 03.12.19.
//

#include "Channel.h"

nio::Channel::Channel(EventLoop* loop,int fd):
                            loop_(loop),
                            fd_(0),
                            events_(0),
                            revents_(0),
                            lastEvents_(0){


}

void nio::Channel::setFd(int fd) { fd_=fd; }

int nio::Channel::getFd() { return fd_; }

void nio::Channel::setEvents(__uint32_t event) {
    events_=event;
}
void nio::Channel::setRevents(__uint32_t event) {
    revents_=event;
}

void nio::Channel::handelEvents() {
    events_=0;
    if((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)){
        return;
    }

    if (revents_ & EPOLLERR) {
        return;
    }

    if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
        handleRead();
    }
    if (revents_ & EPOLLOUT) {
        handleWrite();
    }
    handleConn();
}

void nio::Channel::handleRead() {
    if(readHandler_)
        readHandler_();
}

void nio::Channel::handleWrite() {
    if(writeHandler_)
        writeHandler_();
}

void nio::Channel::handleConn() {
    if(connHandler_)
        connHandler_();
}

void nio::Channel::setReadHandler(CallBack &&readHandler) {
    readHandler_ = readHandler;
}

void nio::Channel::setWriteHandler(CallBack &&writeHandler) {
    writeHandler_=writeHandler;
}

void nio::Channel::setConnHandler(CallBack &&connHandler) {
    connHandler_=connHandler;
}


nio::Channel::~Channel() = default;
