
#include "src/client.h"

#include <thread>

#define SERVER_IP "127.0.0.1"
#define N 2000
using namespace std;

void handle(int j){

    bio::client Client(8000,SERVER_IP);
    Client.start();
    //getchar();
    Client.transmit();

}
int main(int argc, char **argv){

    std::thread ThreadName[N];
    for (int j = 0; j < N; ++j) {
        ThreadName[j] = std::thread(handle, j);
    }
    for(int k = 0; k < N; ++k) {
        ThreadName[k].join();
    }
    return 0;
};