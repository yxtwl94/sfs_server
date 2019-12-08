//
// Created by yxt on 22.11.19.
//
#include <iostream>
#include <getopt.h>

#include "src/Server.h"
#include "src/EventLoop.h"

int main(int argc, char *argv[]) {

    int threadNum=100; //default
    int port=8000;

    int opt;
    const char *str = "t:p:";
    while ((opt = getopt(argc, argv, str)) != -1) {

        switch(opt){

            case 'p':{
                port=atoi(optarg);
                break;
            }
            case 't':{
                threadNum=atoi(optarg);
                break;
            }
            default: break;
        }
    }
    nio::Server server(port);
    printf("Testing Server with port %d\n",port);

    nio::EventLoop a;
    server.setThreadPoolN(threadNum);
    server.start();


    return 0;
}