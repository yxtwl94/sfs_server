//
// Created by yxt on 22.11.19.
//

#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>

namespace bio {

    class server {

    public:
        void setThreadPoolN(size_t n);
        explicit server(int port); //防止隐式转换,类的转换必须显式
        void start();
        static void handler(int arg);
        ~server();

    private:
        size_t threadPoolNum_;
        int port_;
        int listen_fd_;
        int conn_fd_;
        struct sockaddr_in server_addr_;
        struct sockaddr_in client_addr_; // 用于保存客户端地址
    };

}
#endif //SERVER_H
