//
// Created by yxt on 22.11.19.
//
#include <iostream>
#include <zconf.h>
#include <arpa/inet.h>
#include <strings.h>
#include <functional>
#include <fcntl.h>

#include "src/ThreadPool.h"
#include "src/Server.h"
#include "RingBuffer.h"

nio::Server::Server(int port):
                serverLoop_(new nio::EventLoop()),
                serverChannel_(new nio::Channel()),
                threadNum_(0),
                port_(port),
                listen_fd_(0),
                server_addr_(),
                client_addr_(){

    if(port<1024 || port>65535) {
        fprintf(stderr,"port: %d isn't correct!\n", port);
        return;
    }
    bzero((char *)&server_addr_, sizeof(server_addr_)); //数据初始化--清零

    server_addr_.sin_family=AF_INET;
    server_addr_.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr_.sin_port=htons((unsigned short)port);

}


void nio::Server::setThreadPoolN(size_t n) {
    if(n<=0){
        fprintf(stderr,"number of threads in pool must be positive");
        return;
    }
    threadNum_=n;

}

void nio::Server::start() {
    //建立线程池
    if(threadNum_<=0){
        fprintf(stderr,"you must set number of threads in ThreadPool");
        return;
    }
    ThreadPool threadPool(threadNum_);

    /*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
    if((listen_fd_=socket(AF_INET,SOCK_STREAM,0))<0) {
        perror("socket error");
        return;
    }

    // 消除bind时"Address already in use"错误
    auto optVal = 1;
    if (setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &optVal,sizeof(optVal)) == -1) {
        close(listen_fd_);
        return;
    }

    /*将套接字绑定到服务器的网络地址上*/
    if(bind(listen_fd_,(struct sockaddr *)&server_addr_,sizeof(server_addr_))<0){
        close(listen_fd_);
        perror("bind error");
        return;
    }

    /*监听连接请求--监听队列长度为2048*/
    if(listen(listen_fd_,2048)<0) {
        close(listen_fd_);
        perror("listen error");
        return;
    }
    // 无效监听描述符
    if (listen_fd_ < 0 ) {
        close(listen_fd_);
        return;
    }
    if(setNonBlocking(listen_fd_)<0){
        perror("setNonBlocking error");
        return;
    }
    //-------------------------------------------------------------------------------//
    printf("Server Started with fd %d!\n",listen_fd_);

    printf("======waiting for client's request======\n");

    serverChannel_->setEvent(EPOLLIN | EPOLLET);
    serverChannel_->setFd(listen_fd_);
    serverChannel_->setConnHandler(std::bind(&nio::Server::ConnHandler,this));

    serverLoop_->addToPoller(serverChannel_);
    serverLoop_->loop();

}

void nio::Server::ConnHandler(){

    char client_ip[INET_ADDRSTRLEN]=""; //客户端ip,最长16
    struct sockaddr_in client_addr{0};
    bzero((char *)&client_addr, sizeof(client_addr));
    socklen_t cliAddrLen = sizeof(client_addr);   // 必须初始化! 是16

    //边缘触发，要循环
    for(;;){

        int conn_fd = accept(listen_fd_, (struct sockaddr*)&client_addr, &cliAddrLen);
        if(conn_fd==-1){
            //perror("accept error");
            break;
        } //server fd is nonblocking

        //new conn_fd is nonBlocking
        if(nio::Server::setNonBlocking(conn_fd)<0){
            perror("set nonBlocking error");
            return;
        }

        //char greet[]="Hello! I'm a stupid Server\n";
        //send(conn_fd,greet,sizeof greet,0); //greeting
        //add new connection Channel to Poller
        //add new connection Channel to Poller
        nio::Channel::ChannelPtr curChannel(new nio::Channel());
        curChannel->setFd(conn_fd);
        curChannel->setEvent(EPOLLIN | EPOLLET);
        curChannel->setReadHandler(std::bind(&nio::Server::ReadHandler, this, conn_fd));
        serverLoop_->addToPoller(curChannel);

        // 打印客户端的 ip 和端口
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        //printf("--------------------------------------\n");
        printf("new client ip=%s,port=%d,Fd=%d\n", client_ip,ntohs(client_addr.sin_port),conn_fd);
        //printf("--------------------------------------\n");

    }

}

void nio::Server::ReadHandler(int connFd) {
    int done=0;
    for(;;){
        RingBuffer buffer;
        // 从client fd接收数据，写入
        ssize_t byte = buffer.readFromFd(connFd);
        if(byte>0) {
            std::string buf = buffer.getBuffer();
            printf("[%d]received %s\n", connFd, buf.c_str());  //不加'\n'无法输出，呵呵
        }
        if (byte==-1) {
            if (EAGAIN != errno) {
                perror ("[EventLoop]Read data");
                done = 1;
            }
            break;
        }
        else if (byte==0) {
            done = 1;
            break;
        }
    }
    if(done==1){
        printf("[%d]client closed!\n", connFd);
        close(connFd);	//关闭已连接套接字

    }
}


int nio::Server::setNonBlocking(int fd) {

    int flag = fcntl(fd, F_GETFL, 0);
    if (flag == -1) return -1;

    flag |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flag) == -1) return -1;
    return 0;
}

nio::Server::~Server() {
    close(listen_fd_);
    std::cout<<"Server stopped"<<std::endl;
}


