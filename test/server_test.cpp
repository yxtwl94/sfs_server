//
// Created by yxt on 22.11.19.
//
#include <iostream>
#include <getopt.h>

#include "src/Server.h"

int main(int argc, char *argv[]) {

    int threadNum=1000; //default
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
    nio::EventLoop serverLoop;
    nio::Server server(&serverLoop,port,threadNum);
    printf("Testing Server with port %d\n",port);

    server.start();

    return 0;
}