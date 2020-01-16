//
// Created by yxtwl94 on 03.12.19.
//

#include "Channel.h"

nio::Channel::Channel():
                fd_(0),
                event_(0) {

    //fd_=eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC); //just for init
    //assert(fd_>=0);
}

void nio::Channel::startHandling() {
    if(readHandler_) readHandler_();
    if(connHandler_) connHandler_();
    if(writeHandler_) writeHandler_();
}


nio::Channel::~Channel() = default;
