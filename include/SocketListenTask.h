#include <boost/thread/thread.hpp>

class SocketTask {
private:
    int _id;
    ConnectionHandler* ch;
    Protocol* protocol;
public:
    SocketTask(int number, ConnectionHandler* ch, Protocol* protocol) : _id(number), ch(ch), protocol(protocol)  {}
    void operator()(){

        while (1) {
            char nextByte;
            ch->getBytes(&nextByte, 1);
            protocol->readByte(nextByte);
        }
        boost::this_thread::yield(); //Gives up the remainder of the current thread's time slice, to allow other threads to run.
    }
};