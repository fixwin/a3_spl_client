//
// Created by yonatan on 1/11/17.
//

#ifndef A3_SPL_CLIENT_PROTOCOL_H
#define A3_SPL_CLIENT_PROTOCOL_H


#include "Message.h"
#include "connectionHandler.h"

class Protocol {
private:
    bool receiveNextPacket(DataMessage * dm);
    void addBytesToFile(char* b);
    bool ackReceived(AckMessage * m);
    short bytesToShort(char* bytesArr);
    void shortToBytes(short num, char* bytesArr);

    ConnectionHandler* ch;
    char* readBuffer;
    int currentState; // 0-Waiting, 1-SendingFile, 2-ReceivingFile
    short bytesSent, bytesReceived, bytesRemaining, blockNum;
    bool sendZeroBits;
    std::string filename;
public:
    Protocol(ConnectionHandler * ch);
    void process(Message * message);
};


#endif //A3_SPL_CLIENT_PROTOCOL_H
