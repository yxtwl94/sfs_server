//
// Created by yxtwl94 on 2019/12/27.
//

#ifndef SFS_SERVER_EVENTLOOPTHREAD_H
#define SFS_SERVER_EVENTLOOPTHREAD_H

#include <thread>
#include <mutex>

#include "src/EventLoop.h"

namespace nio {

    class EventloopThread{

    public:
        EventloopThread();

        nio::EventLoop *startLoop();

        ~EventloopThread();

    private:
        void threadFunc();
        std::thread thread_;
        nio::EventLoop *loop_;
        std::mutex mutex_;
    };

}


#endif //SFS_SERVER_EVENTLOOPTHREAD_H
