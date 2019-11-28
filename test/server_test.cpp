//
// Created by yxt on 22.11.19.
//
#include <iostream>
#include "src/server.h"

int main(){

    std::cout<<"I'm testing"<<std::endl;
    bio::server server(8000);


    server.setThreadPoolN(100);
    server.start();


    return 0;
}