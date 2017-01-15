#include <stdlib.h>
#include <boost/thread/thread.hpp>
#include <iostream>
#include "../include/ConnectionHandler.h"
#include "../include/KeyboardTask.h"
#include "../include/SocketListenTask.h"


/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);

    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }

    Protocol protocol(&connectionHandler);

    KeayboardTask kt(1, &connectionHandler, &protocol);
    SocketTask st(2, &connectionHandler, &protocol);


    boost::thread th1(kt);
    boost::thread th2(st);

    th1.join();
    th2.join();
    return 0;
}

