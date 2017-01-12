//
// Created by yonatan on 1/10/17.
//

#include "../include/Message.h"

ReadMessage::ReadMessage(string filename) {
    this->opcode=1;
    this->filename=filename;
}

WriteMessage::WriteMessage(string filename) {
    this->opcode=2;
    this->filename = filename;
}

DataMessage::DataMessage(short size, short block, char *bArr) {
    this->opcode=3;
    this->size=size;
    this->block=block;
    this->bArr = bArr;
}

AckMessage::AckMessage(short block) {
    this->opcode=4;
    this->block=block;
}

char *AckMessage::toBytes() {
    char[] ret = char[3];
    return nullptr;
}

ErrMessage::ErrMessage(short errCode, string msg) {
    this->opcode=5;
    this->errCode=errCode;
    this->msg=msg;
}

DirqMessage::DirqMessage() {
    this->opcode=6;
}

LoginMessage::LoginMessage(string username) {
    this->opcode=7;
    this->username=username;
}

DeleteMessage::DeleteMessage(string filename) {
    this->opcode=8;
    this->filename=filename;
}

BcastMessage::BcastMessage(bool added, string filename) {
    this->opcode=9;
    this->added=added;
    this->filename=filename;
}

DisconnectMessage::DisconnectMessage() {
    this->opcode=10;
}
