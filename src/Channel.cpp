//
// Created by yxtwl94 on 03.12.19.
//

#include <sys/eventfd.h>
#include <cassert>

#include "Channel.h"

nio::Channel::Channel():
                fd_(0),
                event_(0) {

    fd_=eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC); //just for init
    assert(fd_>0);
}


nio::Channel::~Channel() = default;
