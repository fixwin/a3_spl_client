#include <boost/thread/thread.hpp>

class SocketTask {
private:
    int _id;
public:
    SocketTask(int number, ConnectionHandler connectionHandler) : _id(number)  {}
    void operator()(){
//        const short bufsize = 1024;
//        char buf[bufsize];

//        while (1) {
//
//        }
        boost::this_thread::yield(); //Gives up the remainder of the current thread's time slice, to allow other threads to run.
    }
};