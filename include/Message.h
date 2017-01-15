//
// Created by yonatan on 1/10/17.
//

#ifndef A3_SPL_CLIENT_MESSAGE_H
#define A3_SPL_CLIENT_MESSAGE_H

#include <string>
#include <stdlib.h>
#include <iostream>
using namespace std;

class Message {
public:
    Message(short opcode) : opcode(opcode){}
    virtual ~Message(){}
    short opcode;
};

class ReadMessage:public Message {
public:
    string filename;
    ReadMessage(string filename) : Message(1), filename(filename) {};
    
};

class WriteMessage:public Message {
public:
    string filename;
    WriteMessage(string filename) : Message(2), filename(filename) {};
    
};

class DataMessage:public Message {
public:
    short size;
    short block;
    char * bArr;
    DataMessage(short size, short block, char bArr[]) : Message(3), size(size), block(block), bArr(bArr) {};
    DataMessage(const DataMessage& dm) : Message(3) , size(dm.size), block(dm.block), bArr(dm.bArr) {};
};

class AckMessage:public Message {
public:
    short block;
    AckMessage(short block) : Message(4), block(block) {};
    
};

class ErrMessage:public Message {
public:
    short errCode;
    string msg;
    ErrMessage(short errCode, string msg) : Message(5), errCode(errCode), msg(msg) {};
    
};

class DirqMessage:public Message {
public:
    DirqMessage() : Message(6) {};
    
};


class LoginMessage:public Message {
public:
    string username;
    LoginMessage(string username) : Message(7), username(username) {};
    
};


class DeleteMessage:public Message {
public:
    string filename;
    DeleteMessage(string filename) : Message(8), filename(filename) {};
    
};

class BcastMessage:public Message {
public:
    string filename;
    bool added;
    BcastMessage(bool added, string filename) : Message(9), filename(filename), added(added) {};
    
};

class DisconnectMessage:public Message {
public:
    DisconnectMessage(): Message(10) {};
    
};
#endif //A3_SPL_CLIENT_MESSAGE_H
