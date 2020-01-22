//
// Created by yxt on 2020/1/15.
//

#ifndef SFS_SERVER_EVENTTHREADPOOL_H
#define SFS_SERVER_EVENTTHREADPOOL_H

#include <src/EventLoop.h>
#include <src/EventLoopThread.h>

namespace nio {
    class EventLoopThreadPool {

    public:
        typedef std::unique_ptr<nio::EventLoopThreadPool> EventLoopThreadPoolPtr;

        EventLoopThreadPool(EventLoop *baseloop, int numThreads);
        void start();
        EventLoop *nextLoop();

        ~EventLoopThreadPool();

    private:
        EventLoop *baseLoop_;
        int numThreads_;
        int next_;
        bool started_;
        std::vector<std::unique_ptr<nio::EventLoopThread>> threads_;
        std::vector<EventLoop*> loops_;

    };
}

#endif //SFS_SERVER_EVENTTHREADPOOL_H
