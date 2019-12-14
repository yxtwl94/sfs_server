//
// Created by yxt on 07.12.19.
//

#ifndef SFS_SERVER_EVENTLOOP_H
#define SFS_SERVER_EVENTLOOP_H

#include <functional>
#include <memory>

#include "Channel.h"
#include "Poller.h"


namespace nio {

    class Channel;
    class Poller;

    class EventLoop {

    public:
        EventLoop();
        ~EventLoop();

        void handleRead(int fd);
        void handleConn(int fd);
        void handleWrite(int fd);

        void addToPoller(const nio::Channel::ChannelPtr& channel) {
            poller_->epollAdd(channel);
        }

        void loop(int serverFd);

    private:
        bool looping_;
        bool quit_;
        std::unique_ptr<nio::Poller> poller_;  //ChannelPtr poller
        //std::shared_ptr<nio::Channel> thisChannel_;
    };

}

#endif //SFS_SERVER_EVENTLOOP_H
