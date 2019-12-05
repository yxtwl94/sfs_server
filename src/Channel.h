//
// Created by yxtw on 03.12.19.
//

#ifndef SFS_SERVER_CHANNEL_H
#define SFS_SERVER_CHANNEL_H

#include <sys/epoll.h>
#include <functional>

namespace nio {

class EventLoop;

class Channel {

public:
    typedef std::function<void()> CallBack;

    Channel(EventLoop *loop, int fd);

    int getFd();
    void setFd(int fd);

    void setEvents(__uint32_t event);
    void setRevents(__uint32_t event);
    void handelEvents();

    void setReadHandler(CallBack &&readHandler);
    void setWriteHandler(CallBack &&writeHandler);
    void setConnHandler(CallBack &&connHandler);

    void handleRead();
    void handleWrite();
    void handleConn();

    ~Channel();


private:

    EventLoop *loop_;
    int fd_;

    __uint32_t events_;
    __uint32_t revents_;
    __uint32_t lastEvents_;

    CallBack readHandler_;
    CallBack writeHandler_;
    CallBack connHandler_;
};


}


#endif //SFS_SERVER_CHANNEL_H
