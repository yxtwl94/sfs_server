//
// Created by yxtwl94 on 07.12.19.
//

#include <cassert>

#include "Epoll.h"

const int EPOLLWAIT_TIME_OUT = 10000;
const int EVENTS_NUM = 4096;

nio::Epoll::Epoll():
            epollFd_(epoll_create1(EPOLL_CLOEXEC)),
            events_(EVENTS_NUM){
    //fd for epoll
    assert(epollFd_>0);
}

void nio::Epoll::epollAdd(const ChannelPtr& channel) {

    int fd=channel->getFd();

    struct epoll_event event{};
    event.data.fd = fd;
    event.events = channel->getEvent();

    channel->updateLastEvents();

    if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event) < 0) {
        perror("ERROR: epoll add");
    }
}

void nio::Epoll::epollDel(const nio::Epoll::ChannelPtr &channel) {

    int fd=channel->getFd();

    struct epoll_event event{};
    event.data.fd = fd;
    event.events = channel->getLastEvents();

    if (epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &event) < 0) {
        perror("ERROR: epoll delete");
    }
}

std::vector<nio::Epoll::ChannelPtr> nio::Epoll::getActChannels() {

    while(True){
        int eventsNum =
                epoll_wait(epollFd_, &*events_.begin(), events_.size(), EPOLLWAIT_TIME_OUT);

        if (eventsNum < 0) {
            perror("ERROR: epoll wait");
        }

        std::vector<nio::Epoll::ChannelPtr> req_data = getEventsRequest(eventsNum);

        if (!req_data.empty()) return req_data;
    }
}

nio::Epoll::~Epoll() = default;