//
// Created by yxt on 07.12.19.
//
#include <zconf.h>
#include <cassert>
#include <arpa/inet.h>

#include "EventLoop.h"
#include "Poller.h"
#include "src/RingBuffer.h"

__thread nio::EventLoop* t_loopInThisThread = nullptr;


nio::EventLoop::EventLoop():
                looping_(false),
                quit_(false),
                poller_(new nio::Poller()) {

    if(t_loopInThisThread){
        //log something
    }
    else{
        t_loopInThisThread=this;
    }


}

void nio::EventLoop::handleConn(int fd) {

    char client_ip[INET_ADDRSTRLEN]=""; //客户端ip,最长16
    struct sockaddr_in client_addr{};
    socklen_t cliAddrLen = sizeof(client_addr);   // 必须初始化! 是16

    //获得一个已经建立的连接,在这之前阻塞
    int conn_fd = accept(fd, (struct sockaddr*)&client_addr, &cliAddrLen);
    if(conn_fd < 0) {
        perror("accept error");
    }

    char greet[]="Hello! I'm a stupid Server\n";
    send(conn_fd,greet,sizeof greet,0); //greeting

    //add new connection Channel to Poller
    nio::Channel::ChannelPtr connChanel(new nio::Channel());
    connChanel->setFd(conn_fd);
    connChanel->setEvent(EPOLLIN | EPOLLET);
    poller_->epollAdd(connChanel);

    // 打印客户端的 ip 和端口
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("--------------------------------------\n");
    printf("new client ip=%s,port=%d\n", client_ip,ntohs(client_addr.sin_port));
    printf("--------------------------------------\n");

}


void nio::EventLoop::handleRead(int fd) {

    RingBuffer buffer;
    // 从client fd接收数据，写入
    ssize_t byte = buffer.readFromFd(fd);
    if(byte>0) {
        std::string buf = buffer.readBuffer();
        printf("[%d]received ", fd);  //不加'\n'无法输出，呵呵
        std::cout << buf << std::endl;
    }

}

void nio::EventLoop::handleWrite(int fd) {
    printf("handle write\n");
}

void nio::EventLoop::loop(int serverFd) {

    assert(!looping_);
    nio::Channel::ChannelList list;

    looping_ = true;
    quit_=false;

    while(!quit_){
        list.clear();
        list = poller_->poll();

        for (auto& it : list) {

            int curFd=it->getFd();
            __uint32_t event=it->getEvent();

            if((event & EPOLLHUP) && !(event & EPOLLIN)){
                return;
            }
            if (event & EPOLLERR) { //文件上发上了一个错误
                return;
            }
            if (event & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) { //文件可读,文件有紧急数据可读,对端关闭连接或者shutdown写入半连接
                if( curFd == serverFd)
                    handleConn(curFd);
                else
                    handleRead(curFd);
            }
            if (event & EPOLLOUT) {  //文件可写
                handleWrite(curFd);
            }
        }
    }
    looping_ = false;
}

nio::EventLoop::~EventLoop() {

    //close(thisChannel_->getFd());
    t_loopInThisThread = nullptr;
}

