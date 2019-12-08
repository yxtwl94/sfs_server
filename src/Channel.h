//
// Created by yxtw on 03.12.19.
//

#ifndef SFS_SERVER_CHANNEL_H
#define SFS_SERVER_CHANNEL_H

#include <sys/epoll.h>
#include <memory>
#include <functional>

namespace nio {

    class EventLoop;

    class Channel {

    public:
        typedef std::function<void()> CallBack;

        explicit Channel(EventLoop *loop);

        int getFd() { return fd_; };

        void setFd(int fd) { fd_ = fd; };

        void setEvent(__uint32_t event) { event_ = event; };

        void setRevent(__uint32_t event) { revent_ = event; };

        __uint32_t getEvent() { return event_; }

        __uint32_t getLastEvents() { return lastEvent_; }

        void handelEvents();

        void setReadHandler(CallBack &&readHandler);

        void setWriteHandler(CallBack &&writeHandler);

        void updateLastEvents() { lastEvent_ = event_; }

        ~Channel();


    private:

        EventLoop *loop_;
        int fd_;

        __uint32_t event_;
        __uint32_t revent_;
        __uint32_t lastEvent_;

        CallBack readHandler_;
        CallBack writeHandler_;
    };

}


#endif //SFS_SERVER_CHANNEL_H
