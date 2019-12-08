//
// Created by yxtwl94 on 07.12.19.
//

#ifndef SFS_SERVER_EPOLL_H
#define SFS_SERVER_EPOLL_H

#include <memory>
#include <vector>

#include "Channel.h"

namespace nio{

    class Epoll {

    public:
        typedef std::shared_ptr<nio::Channel> ChannelPtr;

        Epoll();
        void epollAdd(const ChannelPtr& channel);
        void epollDel(const ChannelPtr& channel);

        std::vector<ChannelPtr> getActChannels();

        ~Epoll();

    private:
        std::vector<epoll_event> events_;
        int epollFd_;
    };

}

#endif //SFS_SERVER_EPOLL_H
