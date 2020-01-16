//
// Created by yxtwl94 on 2019/12/27.
//

#ifndef SFS_SERVER_EVENTLOOPTHREAD_H
#define SFS_SERVER_EVENTLOOPTHREAD_H

#include <thread>
#include <mutex>

#include "src/EventLoop.h"

namespace nio {

    class EventLoopThread{

    public:
        EventLoopThread();
        EventLoop* startLoop();
        ~EventLoopThread();

    private:
        void threadFunc();
        std::thread thread_;
        nio::EventLoop *loop_;
        std::mutex mutex_;
    };

}


#endif //SFS_SERVER_EVENTLOOPTHREAD_H
