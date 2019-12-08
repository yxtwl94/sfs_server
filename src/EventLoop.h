//
// Created by yxt on 07.12.19.
//

#ifndef SFS_SERVER_EVENTLOOP_H
#define SFS_SERVER_EVENTLOOP_H

#include <functional>
#include <memory>

#include "Channel.h"
#include "Epoll.h"


namespace nio {

    class Channel;
    class Epoll;

    class EventLoop {

    public:
        EventLoop();
        ~EventLoop();

        void handleRead();

        void loop();

    private:
        bool looping_;

        std::unique_ptr<nio::Epoll> poller_;  //ChannelPtr poller
        std::shared_ptr<nio::Channel> thisChannel_;
    };

}

#endif //SFS_SERVER_EVENTLOOP_H
