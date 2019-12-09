//
// Created by yxt on 07.12.19.
//
#include <zconf.h>
#include <cassert>
#include <arpa/inet.h>

#include "EventLoop.h"
#include "Poller.h"


__thread nio::EventLoop* t_loopInThisThread = nullptr;


nio::EventLoop::EventLoop():
                looping_(false),
                quit_(false),
                poller_(new nio::Poller()),
                thisChannel_(new nio::Channel(this)) {

    if(t_loopInThisThread){
        //log something
    }
    else{
        t_loopInThisThread=this;
    }

    //epoll create steps
    poller_->epollAdd(thisChannel_);

}

void nio::EventLoop::handleRead(int fd) {
    char client_ip[INET_ADDRSTRLEN]=""; //客户端ip,最长16
    struct sockaddr_in client_addr{};
    socklen_t cliAddrLen = sizeof(client_addr);   // 必须初始化! 是16

    //获得一个已经建立的连接,在这之前阻塞
    int conn_fd = accept(fd, (struct sockaddr*)&client_addr, &cliAddrLen);
    if(conn_fd < 0) {
        perror("accept error");
    }

    // 打印客户端的 ip 和端口
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("--------------------------------------\n");
    printf("new client ip=%s,port=%d\n", client_ip,ntohs(client_addr.sin_port));
    printf("--------------------------------------\n");

}

void nio::EventLoop::handleWrite(int fd) {
    printf("[%d]i'm handling write\n",fd);
}

void nio::EventLoop::loop() {

    assert(!looping_);
    nio::Channel::ChannelList list;

    looping_ = true;
    quit_=false;

    while(!quit_){
        list.clear();
        list = poller_->poll();

        for (auto& it : list) {

            __uint32_t event=it->getEvent();
            if((event & EPOLLHUP) && !(event & EPOLLIN)){
                return;
            }
            //文件上发上了一个错误
            if (event & EPOLLERR) {
                return;
            }
            //文件可读,文件有紧急数据可读,对端关闭连接或者shutdown写入半连接
            if (event & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
                handleRead(it->getFd());
            }
            //文件可写
            if (event & EPOLLOUT) {
                handleWrite(it->getFd());
            }
        }
    }
    looping_ = false;
}

nio::EventLoop::~EventLoop() {
    close(thisChannel_->getFd());
    t_loopInThisThread = nullptr;
}
