//
// Created by yxtwl94 on 07.12.19.
//

#ifndef SFS_SERVER_POLLER_H
#define SFS_SERVER_POLLER_H

#include <memory>
#include <vector>
#include <map>

#include "Channel.h"

namespace nio{

    class Poller {

    public:
        //typedef std::shared_ptr<nio::Channel> ChannelPtr;
        //typedef std::vector<ChannelPtr> ChannelList; //事件指针数组类型


        Poller();
        ~Poller();

        nio::Channel::ChannelList poll();    //等待事件，epoll_wait封装
        void epollAdd(const nio::Channel::ChannelPtr& channel);  //添加事件
        void epollDel(const nio::Channel::ChannelPtr& channel);  //移除事件


    private:
        std::vector<struct epoll_event> events_;  //events数组，用于传递给epollwait
        std::map<int, nio::Channel::ChannelPtr> channelMap_; //事件表
        int epollFd_;
    };

}

#endif //SFS_SERVER_POLLER_H
