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

        void addToPoller(const nio::Channel::ChannelPtr& channel) {
            poller_->epollAdd(channel);
        }

        void loop();

    private:

        std::unique_ptr<nio::Poller> poller_;  //ChannelPtr poller
    };

}

#endif //SFS_SERVER_EVENTLOOP_H
