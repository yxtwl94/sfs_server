//
// Created by yxtwl94 on 2019/12/27.
//

#ifndef SFS_SERVER_EVENTLOOPTHREAD_H
#define SFS_SERVER_EVENTLOOPTHREAD_H

#include <thread>
#include <mutex>
#include <condition_variable>

#include "src/EventLoop.h"

namespace nio {

    class EventLoopThread{

    public:
        EventLoopThread();
        EventLoop* threadLoop();
        ~EventLoopThread();

    private:
        void threadFunc();
        std::thread thread_;
        nio::EventLoop *loop_;
        std::mutex mutex_;
        std::condition_variable cond_;
    };

}


#endif //SFS_SERVER_EVENTLOOPTHREAD_H
