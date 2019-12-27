//
// Created by parallels on 11/24/19.
//

#include <iostream>
#include <zconf.h>
#include <arpa/inet.h>
#include <strings.h>
#include <cstring>
#include <chrono>
#include <thread>
#include "src/client.h"

bio::client::client(int port, const char* serverip):
        port_(port),
        my_addr_(),
        remote_addr_(),
        serverip_(serverip),
        conn_fd_(0),
        len_(0),
        buf(""){
    if(port < 1024 || port > 65535){
        fprintf(stderr, "port: %d isn't correct!\n", port);
        return;
    }
    //printf("Client Started at port %d!\n", port);
    bzero((char *)&remote_addr_, sizeof(remote_addr_));

    remote_addr_.sin_family = AF_INET;
    remote_addr_.sin_addr.s_addr = inet_addr(serverip_);
    remote_addr_.sin_port = htons(port);

}

void bio::client::start() {

    if((conn_fd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket error");
        return;
    }

    if(connect(conn_fd_, (struct sockaddr *)&remote_addr_, sizeof(struct sockaddr)) < 0){
        perror("connect error");
        return;
    }
    printf("[%d]connected to server\n",conn_fd_);
    //len_ = recv(conn_fd_, buf, BUFSIZE, 0);//接受服务器端信息
    //buf[len_] = '\0';
    //printf("%s", buf);
    //close(conn_fd_);
}

void bio::client::transmit(){

    for(int i=0;i<3;++i)
    {
        std::chrono::seconds duration(1);
        std::this_thread::sleep_for(duration);

        char input[] = "stupid!\n";
        if(!strcmp(input, "quit"))
            break;
        send(conn_fd_, input, strlen(input), 0);
        //len_ = recv(conn_fd_, buf, BUFSIZE, 0);
        //buf[len_] = '\0';
        //printf("received: %s\n", buf);

    }
}

bio::client::~client() {
    close(conn_fd_);
    printf("[%d]Client is closed\n",conn_fd_);
}
