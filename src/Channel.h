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

        typedef std::shared_ptr<Channel> ChannelPtr;
        typedef std::function<void()> CallBack;
        typedef std::vector<ChannelPtr> ChannelList; //事件指针数组类型

        Channel();

        int getFd() { return fd_; };

        void setFd(int fd) { fd_ = fd; };

        void setEvent(__uint32_t event) { event_ = event; };

        void setConnHandler(CallBack &&connHandler) { connHandler_ = connHandler; }

        void setReadHandler(CallBack &&readHandler) { readHandler_ = readHandler; }

        void setWriteHandler(CallBack &&writeHandler) { connHandler_ = writeHandler; }

        void startHandling();

        __uint32_t getEvent() { return event_; }

        ~Channel();


    private:
        CallBack readHandler_;
        CallBack connHandler_;
        CallBack writeHandler_;
        int fd_;
        __uint32_t event_;

    };

}


#endif //SFS_SERVER_CHANNEL_H
