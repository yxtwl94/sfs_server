//
// Created by parallels on 11/26/19.
//

#include "src/client.h"

#include <iostream>
#include <thread>
#include <functional>
#include <vector>

#define SERVER_IP "172.20.10.5"
#define N 3000
using namespace std;

std::vector<bio::client> ClientName;

void handle(int j){

    std::cout<<endl<<j<<endl;
    ClientName[j].start();
    ClientName[j].transmit();


}
int main(int argc, char **argv){

    for (int i = 0; i < N; ++i) {
        bio::client ClientName_temp(8000,SERVER_IP);
        ClientName.push_back(ClientName_temp);
    }
    std::thread ThreadName[N];
    for (int j = 0; j < N; ++j) {
        ThreadName[j] = std::thread(handle, j);
    }
    for(int k = 0; k < N; ++k){
        ThreadName[k].join();
    }
    //getchar();

    return 0;
};

