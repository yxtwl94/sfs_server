//
// Created by yxt on 2019/12/27.
//

#include "EventLoopThread.h"

nio::EventLoopThread::EventLoopThread():
                        loop_(nullptr),
                        thread_(std::thread(&nio::EventLoopThread::threadFunc, this)){

}

void nio::EventLoopThread::threadFunc() {
    nio::EventLoop loop;

    {
        std::unique_lock<std::mutex> lock(mutex_);
        loop_=&loop;
    }
    loop.loop();
    loop_ = nullptr;
    return;
}

nio::EventLoopThread::~EventLoopThread() {

}

nio::EventLoop* nio::EventLoopThread::startLoop() {

    {
        std::unique_lock<std::mutex> lock(mutex_);

    }
    return loop_;
}
