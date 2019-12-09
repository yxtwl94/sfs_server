//
// Created by yxtw on 03.12.19.
//

#ifndef SFS_SERVER_CHANNEL_H
#define SFS_SERVER_CHANNEL_H

#include <sys/epoll.h>
#include <memory>
#include <functional>
#include <vector>

namespace nio {

    class EventLoop;

    class Channel {

    public:
        typedef std::shared_ptr<nio::Channel> ChannelPtr;
        typedef std::vector<ChannelPtr> ChannelList; //事件指针数组类型

        explicit Channel(EventLoop *loop);

        int getFd() { return fd_; };

        void setFd(int fd) { fd_ = fd; };

        void setEvent(__uint32_t event) { event_ = event; };

        __uint32_t getEvent() { return event_; }


        ~Channel();


    private:
        int fd_;
        EventLoop *loop_;
        __uint32_t event_;

    };

}


#endif //SFS_SERVER_CHANNEL_H
