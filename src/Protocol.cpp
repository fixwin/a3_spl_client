//
// Created by yonatan on 1/11/17.
//

#include <fstream>
#include "../include/Protocol.h"

Protocol::Protocol(ConnectionHandler *ch) : ch(ch){

}

void Protocol::process(Message * message) {
    short opCode = message->opcode;
    switch (opCode) {

        case 3:
            DataMessage* dm = (DataMessage*) message;
            if(receiveNextPacket(dm)) {//if was the last packet
//                try {


                FILE *pFile;
                pFile = fopen (filename, "w");
                fwrite (readBuffer , 1 , bytesReceived , pFile );

                std::cout << "RRQ " << " complete" << std::endl;

                filename = "";
                bytesReceived=0;
                currentState = 0; //Waiting
//                }
//                catch () {
//
//                }
            }
            else {
                AckMessage ackMessage(bytesReceived/512+1);
                ch->sendBytes()
            }
            break;
        case 4:
            ackReceived((AckMessage*) message);
            break;
        case 5:
            //TODO Error
            break;
        case 9:
            //TODO BCAST
            break;
    }
}

bool Protocol::receiveNextPacket(DataMessage *dm) {
    bool ret = false;
    if(currentState == 2) {//ReceivingFile
        if(dm->size != 0) {
            addBytesToFile(dm->bArr);
        }
        if(dm->size < 512) {
            currentState = 0; // Waiting
            ret = true;
        }
    }
    bytesReceived+=dm->size;
    return ret;
}

bool Protocol::ackReceived(AckMessage *m) {
    std::cout << "ACK" << m->block << std::endl;
    switch (currentState) {
        case 1: //SendingFile
            if (!sendNextPacket()) {
                currentState = 0; //Waiting
            }
            break;
    }

    return  true;
}

short Protocol::bytesToShort(char *bytesArr) {
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}

void Protocol::shortToBytes(short num, char *bytesArr) {
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}

void Protocol::addBytesToFile(char *b) {
 //TODO
}
