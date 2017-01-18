//
// Created by yonatan on 1/11/17.
//

#ifndef A3_SPL_CLIENT_PROTOCOL_H
#define A3_SPL_CLIENT_PROTOCOL_H

class ConnectionHandler;

#include "Message.h"
#include "ConnectionHandler.h"


class Protocol {
private:
    bool receiveNextPacket(DataMessage *dm);

    bool sendNextPacket();

    bool ackReceived(AckMessage *m);

    short bytesToShort(char *bytesArr);

    void shortToBytes(short num, char *bytesArr);

    ConnectionHandler *ch;
    vector<char> readBuffer;
    char *writeBuffer;
    int currentState = Waiting; // 0-WaitingIdle, 1-SendingFile, 2-ReceivingFile
    int bytesSent = 0, bytesReceived = 0, bytesRemaining = 0;
    short blockNum = 0;
    bool sendZeroBits = false;
    std::string filename;

    int counter = 0;
    char shortArray[2];
    short opCode = 0;
    vector<char> readerArr;
    short packetSize = 0;
    short errorCode = 0;
    bool written = false;

    void errorClear();

public:
    Protocol(ConnectionHandler *ch) : ch(ch), readBuffer(), writeBuffer(), filename(), readerArr() {};

    Protocol(const Protocol &p) : ch(p.ch), readBuffer(), writeBuffer(), filename(), readerArr() {};

    void process(Message *message);

    void sendMessage(std::string mes);

    void readByte(char ch);

    enum State {
        Waiting, SendingFile, ReceivingFile, Dirq
    };
};


#endif //A3_SPL_CLIENT_PROTOCOL_H
