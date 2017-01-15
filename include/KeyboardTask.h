#include <boost/thread/thread.hpp>

class KeayboardTask {
private:
    int _id;
    ConnectionHandler* ch;
    Protocol* protocol;
public:
    KeayboardTask(int number, ConnectionHandler* ch, Protocol* protocol) : _id(number) , ch(ch) , protocol(protocol){}

    void operator()(){
        while (1) {
            const short bufsize = 1024;
            char buf[bufsize];

//            char a[6] = {0,7,'y','o','n','\0'};
//
//            ch->sendBytes(a,6);

            std::cin.getline(buf, bufsize);
            std::string line(buf);

            protocol->sendMessage(line);
        }
        boost::this_thread::yield(); //Gives up the remainder of the current thread's time slice, to allow other threads to run.
    }

};