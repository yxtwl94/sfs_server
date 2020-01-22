//
// Created by yxt on 2019/12/27.
//

#include "EventLoopThread.h"

nio::EventLoopThread::EventLoopThread():
                        loop_(nullptr),
                        thread_(std::thread(&nio::EventLoopThread::threadFunc, this)){

}

void nio::EventLoopThread::threadFunc() {

    EventLoop loop;
    //create one new threadLoop for this thread
    {
        std::unique_lock<std::mutex> lock(mutex_);
        loop_=&loop;
        cond_.notify_one();
    }
    loop.loop();

    std::unique_lock<std::mutex> lock(mutex_);
    loop_ = nullptr;
}

nio::EventLoop* nio::EventLoopThread::threadLoop() {

    {
        std::unique_lock<std::mutex> lock(mutex_);
        while(loop_== nullptr){
            cond_.wait(lock);
        }
    }
    return loop_;
}

nio::EventLoopThread::~EventLoopThread() {
    if(loop_!= nullptr){
        thread_.join();
    }
}
//i love u too