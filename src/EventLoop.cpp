//
// Created by yxt on 07.12.19.
//
#include <zconf.h>
#include <cassert>

#include "EventLoop.h"


__thread nio::EventLoop* t_loopInThisThread = nullptr;


nio::EventLoop::EventLoop():
                looping_(false),
                poller_(new nio::Epoll()),
                thisChannel_(new nio::Channel(this)) {

    if(t_loopInThisThread){
        //log something
    }
    else{
        t_loopInThisThread=this;
    }

    //Channel init
    //std::cout<<(EPOLLIN | EPOLLET);
    thisChannel_->setEvent(EPOLLIN | EPOLLET); //Edge triggered
    thisChannel_->setReadHandler(std::bind(&EventLoop::handleRead, this));

    //epoll create steps
    poller_->epollAdd(thisChannel_);

}

void nio::EventLoop::handleRead() {
    thisChannel_->setEvent(EPOLLIN | EPOLLET);
}


void nio::EventLoop::loop() {

    assert(!looping_);

}

nio::EventLoop::~EventLoop() {
    close(thisChannel_->getFd());
    t_loopInThisThread = nullptr;
}
