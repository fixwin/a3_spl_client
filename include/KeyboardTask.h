#include <boost/thread/thread.hpp>

class KeayboardTask {
private:
    int _id;
    ConnectionHandler* ch;
public:
    KeayboardTask(int number, ConnectionHandler* ch) : _id(number) , ch(ch) {}

    void operator()(){
        while (1) {
            const short bufsize = 1024;
            char buf[bufsize];

            char a[6] = {0,7,'y','o','n','\0'};

            ch->sendBytes(a,6);

            std::cin.getline(buf, bufsize);
            std::string line(buf);
//            int len=line.length();
//            std::cout << line << std::endl;
            if (!ch->sendLine(line)) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }

            std::string answer;
            if (!ch->getLine(answer)) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
            std::cout << answer <<  std::endl;
        }
        boost::this_thread::yield(); //Gives up the remainder of the current thread's time slice, to allow other threads to run.
    }
};