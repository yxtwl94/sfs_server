//
// Created by yxt on 07.12.19.
//
#include <zconf.h>
#include <arpa/inet.h>

#include "EventLoop.h"
#include "Poller.h"

__thread nio::EventLoop* t_loopInThisThread = nullptr;


nio::EventLoop::EventLoop():
                poller_(new nio::Poller()) {

    if(t_loopInThisThread){
        //log something
    }
    else{
        t_loopInThisThread=this;
    }

}

void nio::EventLoop::loop() {

    //assert(!looping_);
    nio::Channel::ChannelList activeChanList;
    //quit_=false;

    for(;;){
        activeChanList.clear();
        activeChanList = poller_->poll();

        for (auto& it : activeChanList) {
            int fd=it->getFd();
            __uint32_t event=it->getEvent();

            if( event & EPOLLERR || event & EPOLLHUP || (! (event & EPOLLIN))){
                //server文件上发上了一个错误,对已关闭客户端写入等错误
                nio::Channel::ChannelPtr curChannel(new nio::Channel());
                curChannel->setFd(fd);
                poller_->epollDel(curChannel);
                perror("Epoll Error");
                printf("[%d]client closed!\n", fd);
                close(fd);
                //return;
            }
            else if ((event & EPOLLIN) || (event & EPOLLPRI) || event & (EPOLLRDHUP)) {
                //文件可读,文件有紧急数据可读,对端关闭连接或者shutdown
                it->startHandling();
            }
            else if (event & EPOLLOUT) {  //文件可写

            }
        }
    }
}

nio::EventLoop::~EventLoop() {

    t_loopInThisThread = nullptr;
}

