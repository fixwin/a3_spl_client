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
    short opcode;
    virtual char* toBytes()=0;
};

class ReadMessage:public Message {
public:
    string filename;
    ReadMessage(string filename);
    virtual char* toBytes();
};

class WriteMessage:public Message {
public:
    string filename;
    WriteMessage(string filename);
    virtual char* toBytes();
};

class DataMessage:public Message {
public:
    short size;
    short block;
    char * bArr;
    DataMessage(short size, short block, char bArr[]);
    virtual char* toBytes();
};

class AckMessage:public Message {
public:
    short block;
    AckMessage(short block);
    virtual char* toBytes();
};

class ErrMessage:public Message {
public:
    short errCode;
    string msg;
    ErrMessage(short errCode, string msg);
    virtual char* toBytes();
};

class DirqMessage:public Message {
public:
    DirqMessage();
    virtual char* toBytes();
};


class LoginMessage:public Message {
public:
    string username;
    LoginMessage(string username);
    virtual char* toBytes();
};


class DeleteMessage:public Message {
public:
    string filename;
    DeleteMessage(string filename);
    virtual char* toBytes();
};

class BcastMessage:public Message {
public:
    string filename;
    bool added;
    BcastMessage(bool added, string filename);
    virtual char* toBytes();
};

class DisconnectMessage:public Message {
public:
    DisconnectMessage();
    virtual char* toBytes();
};
#endif //A3_SPL_CLIENT_MESSAGE_H
