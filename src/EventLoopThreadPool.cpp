//
// Created by yxt on 2020/1/15.
//

#include <cassert>

#include "EventLoopThreadPool.h"


nio::EventLoopThreadPool::EventLoopThreadPool(nio::EventLoop *baseLoop, int numThreads)
:baseLoop_(baseLoop), numThreads_(numThreads) ,started_(false) {
    //LOG some
    if(numThreads<=0) abort();
}


void nio::EventLoopThreadPool::start() {

    started_=true;
    for(int i=0;i<numThreads_;++i){
        std::shared_ptr<nio::EventLoopThread> t(new nio::EventLoopThread());
        threads_.emplace_back(t);
        loops_.emplace_back(t->startLoop());
    }
}

nio::EventLoop *nio::EventLoopThreadPool::nextLoop() {
    assert(started_);
    nio::EventLoop *loop = baseLoop_;
    if(!loops_.empty()){
        loop=loops_[next_];
        next_=(next_+1)%numThreads_;
    }
    return loop;
}

nio::EventLoopThreadPool::~EventLoopThreadPool() = default;
