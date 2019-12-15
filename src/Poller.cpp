//
// Created by yxtwl94 on 07.12.19.
//

#include <cassert>
#include <zconf.h>

#include "Poller.h"

const int EPOLLWAIT_TIMEOUT = 10000;  //epoll_wait 超时时间设置
const int EVENTS_NUM = 4096;          //最大触发事件数量

nio::Poller::Poller():
            epollFd_(epoll_create1(EPOLL_CLOEXEC)),
            events_(EVENTS_NUM){
    //fd for epoll
    assert(epollFd_>0);
}

void nio::Poller::epollAdd(const nio::Channel::ChannelPtr& channel) {

    int fd=channel->getFd();

    struct epoll_event event{};
    event.data.fd = fd;
    event.events = channel->getEvent();

    channelMap_[fd]=channel;

    if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event) < 0) {
        channelMap_[fd].reset(); //计数减１
        perror("ERROR: epoll add");
    }
}

void nio::Poller::epollDel(const nio::Channel::ChannelPtr &channel) {

    int fd=channel->getFd();

    struct epoll_event event{};
    event.data.fd = fd;
    event.events = channel->getEvent();

    if (epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &event) < 0) {
        perror("ERROR: epoll delete");
    }
    channelMap_[fd].reset(); //计数减１
}


nio::Channel::ChannelList nio::Poller::poll() {

    int timeout = EPOLLWAIT_TIMEOUT;

    nio::Channel::ChannelList chanList;
    //监控epollFd结构体的事件并返回事件到events_
    int nfds = epoll_wait(epollFd_, &*events_.begin(), events_.size(), timeout);
    if(nfds < 0) {
        perror("ERROR: epoll wait");
    }
    //将所有事件添加到chanList里方便EventLoop使用
    for(size_t i=0;i<nfds;++i){
        int event=events_[i].events;
        int fd = events_[i].data.fd;

        nio::Channel::ChannelPtr cur_chanel = channelMap_[fd];
        if(cur_chanel){
            cur_chanel->setEvent(event);    //将发生事件的Channel加进去,每个Channel有独立的Fd
        }
        chanList.emplace_back(cur_chanel);
    }
    return chanList;

}

nio::Poller::~Poller() {

    close(epollFd_);
}