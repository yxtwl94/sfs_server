//
// Created by yxtwl94 on 2019/12/27.
//

#include "EventloopThread.h"

nio::EventloopThread::EventloopThread():
                        loop_(nullptr),
                        thread_(std::thread(&nio::EventloopThread::threadFunc,this)){

}

nio::EventLoop *nio::EventloopThread::startLoop() {
    return nullptr;
}

void nio::EventloopThread::threadFunc() {
    nio::EventLoop loop;

    {
        std::unique_lock<std::mutex> lock(mutex_);
        loop_=&loop;

    }
    loop.loop();
    loop_ = nullptr;
    return;
}

nio::EventloopThread::~EventloopThread() {

}
