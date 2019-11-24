//
// Created by yxt on 22.11.19.
//
#include <iostream>
#include <zconf.h>
#include <arpa/inet.h>
#include <thread>
#include <strings.h>
#include <cstring>

#include "src/server.h"

#define MAX_BUFF 2048

bio::server::server(int port):
                port_(port),
                server_addr_(),
                client_addr_(),
                listen_fd_(0),
                conn_fd_(0) {

    if(port<0 || port>65535) {
        fprintf(stderr,"port: %d isn't correct!\n", port);
        return;
    }
    printf("Server Started at port %d!\n", port);
    bzero((char *)&server_addr_, sizeof(server_addr_)); //数据初始化--清零

    server_addr_.sin_family=AF_INET;
    server_addr_.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr_.sin_port=htons((unsigned short)port);

}

void bio::server::start() {
    /*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
    if((listen_fd_=socket(AF_INET,SOCK_STREAM,0))<0) {
        perror("socket error");
        return;
    }

    /*将套接字绑定到服务器的网络地址上*/
    if(bind(listen_fd_,(struct sockaddr *)&server_addr_,sizeof(server_addr_))<0){
        perror("bind error");
        return;
    }

    /*监听连接请求--监听队列长度为2048*/
    if(listen(listen_fd_,2048)<0) {
        perror("listen error");
        return;
    }
    // 无效监听描述符
    if (listen_fd_ < 0 ) {
        close(listen_fd_);
        return;
    }
    printf("======waiting for client's request======\n");

    while(true) {
        char client_ip[INET_ADDRSTRLEN]=""; //客户端ip,最长16
        socklen_t cliaddr_len = sizeof(client_addr_);   // 必须初始化! 是16

        //获得一个已经建立的连接,在这之前阻塞
        conn_fd_ = accept(listen_fd_, (struct sockaddr*)&client_addr_, &cliaddr_len);
        if(conn_fd_ < 0) {
            perror("accept error");
            continue;
        }

        // 打印客户端的 ip 和端口
        inet_ntop(AF_INET, &client_addr_.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("--------------------------------------\n");
        printf("new client ip=%s,port=%d\n", client_ip,ntohs(client_addr_.sin_port));

        if(conn_fd_ > 0) {
            std::thread serverThread(handler,conn_fd_);
            std::cout<<"start Thread with conn_fd "<<conn_fd_<<std::endl;
            serverThread.detach(); //不用等线程,直接返回处理下一个连接
        }
    }
}

void bio::server::handler(int arg) {

    int conn_fd = arg;
    char greet[]="Hello! I'm a stupid server,use 'exit' to get out\n";
    send(conn_fd,greet,sizeof(greet),0); //greeting

    // 从client fd接收数据，写入recv_buf里
    while (true) {
        char buff[MAX_BUFF]=""; // 接收缓冲区,必须每次初始化!
        int byte=recv(conn_fd, buff, sizeof(buff), 0);

        if(byte>0){
            if(strncmp(buff, "exit",4) == 0) break; //只比较前4个,输入字符进入缓冲区后貌似会多两个

            printf("[%d]received buf is %s",conn_fd,buff);
            send(conn_fd,buff,sizeof(buff),0); //反馈给客户端
        }
        else if(byte==0) break; //网络中断返回0
        else{
            perror("receive error");
        }
    }
    printf("\nclient[conn_fd:%d] closed!\n", conn_fd);
    close(conn_fd);	//关闭已连接套接字

}

bio::server::~server() {
    close(listen_fd_);
    std::cout<<"server stopped"<<std::endl;
}
