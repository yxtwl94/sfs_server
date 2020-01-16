//
// Created by yxt on 22.11.19.
//

#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>

#include "src/EventLoopThreadPool.h"
#include "src/EventLoop.h"
#include "src/Channel.h"


namespace nio {

    class Server {

    public:
        Server(EventLoop *serverLoop,int port,int threadNum); //防止隐式转换,类的转换必须显式
        void start();
        void ConnHandler();
        void ReadHandler(int connFd);
        int setNonBlocking(int fd);

        //static void handler(int arg);
        ~Server();

    private:
        nio::EventLoopThreadPool::EventLoopThreadPoolPtr eventLoopThreadPool_;
        nio::Channel::ChannelPtr serverChannel_;
        EventLoop* serverLoop_;
        int port_;
        int listen_fd_;
        struct sockaddr_in server_addr_;
        struct sockaddr_in client_addr_; // 用于保存客户端地址
    };

}
#endif //SERVER_H
