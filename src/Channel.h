//
// Created by yxtw on 03.12.19.
//

#ifndef SFS_SERVER_CHANNEL_H
#define SFS_SERVER_CHANNEL_H

#include <sys/epoll.h>


namespace nio {

class EventLoop;

class Channel {

public:
    Channel(EventLoop *loop, int fd);

    Ｃhannel(EventLoop* loop,int fd);

    ~Channel()


private:

};


}


#endif //SFS_SERVER_CHANNEL_H
