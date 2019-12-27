
//
// Created by parallels on 11/24/19.
//

#ifndef SFS_SERVER_CLIENT_H
#define SFS_SERVER_CLIENT_H

#include <iostream>
#include <zconf.h>
#include <arpa/inet.h>
#include <strings.h>
#include <cstring>
#include <sys/socket.h>
#include <stdio.h>

#define BUFSIZE 2048
namespace bio {

    class client {

    public:
        explicit client(int port, const char* serverip);
        void start();

        void transmit();
        ~client();

    private:
        int port_;
        int conn_fd_;
        int len_;
        const char* serverip_;
        struct sockaddr_in my_addr_;
        struct sockaddr_in remote_addr_;
        char buf[BUFSIZE];



    };
}

#endif //SFS_SERVER_CLIENT_H
