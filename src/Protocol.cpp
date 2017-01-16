//
// Created by yonatan on 1/11/17.
//

#include <fstream>
#include "../include/Protocol.h"

//Protocol::Protocol(ConnectionHandler *ch) : ch(ch) {
////    ch->setProtocol(this);
//}

void Protocol::readByte(char nextByte) {
    if (counter < 2) {
        shortArray[counter] = nextByte;
        if (counter == 1) {
            opCode = bytesToShort(shortArray);
        }
    } else {
        switch (opCode) {
            case 3: {
                if (counter < 4) {
                    shortArray[counter - 2] = nextByte;
                    if (counter == 3) {
                        packetSize = bytesToShort(shortArray);
                    }
                } else if (counter < 6) {
                    shortArray[counter - 4] = nextByte;
                    if (counter == 5) {
                        blockNum = bytesToShort(shortArray);
                        if (packetSize == 0) {
                            DataMessage m(packetSize, blockNum, readerArr.data());
                            readerArr.clear();
                            counter = 0;
                            process(&m);
                            return;
                        }
                    }
                } else if (counter < 6 + packetSize) { //if reading data
                    readerArr.push_back(nextByte);
                    if (counter == 6 + packetSize - 1) {
                        DataMessage m(packetSize, blockNum, readerArr.data());
                        readerArr.clear();
                        counter = 0;
                        process(&m);
                        return;
                    }
                } else { //end of file
                    // crate packet
                    DataMessage m(packetSize, blockNum, readerArr.data());
                    readerArr.clear();
                    counter = 0;
                    process(&m);
                    return;
                }
                break;
            }
            case 4: {
                if (counter < 4) {
                    shortArray[counter - 2] = nextByte;
                    if (counter == 3) {
                        packetSize = bytesToShort(shortArray);
                        AckMessage m(packetSize);
                        counter = 0;
                        process(&m);
                        return;
                    }
                }
                break;
            }
            case 5: {
                if (counter < 4) {
                    shortArray[counter - 2] = nextByte;
                    if (counter == 3) {
                        errorCode = bytesToShort(shortArray);
                    }
                } else if (nextByte != '\0') {
                    readerArr.push_back(nextByte);
                } else { //if finished
                    //add error
                    readerArr.push_back(nextByte);
                    ErrMessage m(errorCode, readerArr.data());
                    readerArr.clear();
                    counter = 0;
                    process(&m);
                    return;
                }
                break;
            }
            case 9: {
                if (counter == 2) {
                    written = nextByte;
                } else if (nextByte != '\0') {
                    readerArr.push_back(nextByte);
                } else {
                    readerArr.push_back(nextByte);
                    BcastMessage m(written, readerArr.data());
                    readerArr.clear();
                    counter = 0;
                    process(&m);
                    return;
                }
                break;
            }
        }
    }
    counter++;
}

void Protocol::sendMessage(std::string message) {
    string mes = message.substr(0, message.size() + 1);
    int findLen = mes.find(" ");
    string op = mes.substr(0, findLen);
    if (findLen != -1)
        mes.erase(0, findLen + 1);
    if (op == "RRQ") {
        currentState = ReceivingFile;
        filename = mes;
        char send[2 + mes.size() + 1];
        send[0] = 0;
        send[1] = 1;
        for (unsigned int i = 0; i < mes.size(); ++i) {
            send[2 + i] = mes.at(i);
        }
        send[2 + mes.size()] = 0;
        ch->sendBytes(send, 2 + mes.size() + 1);
    } else if (op == "WRQ") {
        currentState = SendingFile;
        filename = mes;
        bytesSent = 0;
        blockNum = 1;

        ifstream myfile(filename.c_str());
//        myfile.open(filename.c_str());
        if (!myfile.is_open()) {
            cout << "file does not exist" << endl;
            return;
        }
        myfile.seekg(0, std::ios::end);
        size_t fileSize = myfile.tellg();
        writeBuffer = new char[fileSize];
        myfile.seekg(0, std::ios::beg);
        myfile.read(writeBuffer, fileSize);
        myfile.close();

        bytesRemaining = fileSize;
        sendZeroBits = bytesRemaining%512 == 0;
        char send[2 + mes.size() + 1];
        send[0] = 0;
        send[1] = 2;
        for (unsigned int i = 0; i < mes.size(); ++i) {
            send[2 + i] = mes.at(i);
        }
        send[2 + mes.size()] = 0;
        ch->sendBytes(send, 2 + mes.size() + 1);
    } else if (op == "DIRQ") {
        currentState = Dirq;
        char send[2] = {0, 6};
        ch->sendBytes(send, 2);
    } else if (op == "LOGRQ") {
        char send[2 + mes.size() + 1];
        send[0] = 0;
        send[1] = 7;
        for (unsigned int i = 0; i < mes.size(); ++i) {
            send[2 + i] = mes.at(i);
        }
        send[2 + mes.size()] = 0;
        ch->sendBytes(send, 2 + mes.size() + 1);
    } else if (op == "DELRQ") {
        filename = mes;
        char send[2 + mes.size() + 1];
        send[0] = 0;
        send[1] = 8;
        for (unsigned int i = 0; i < mes.size(); ++i) {
            send[2 + i] = mes.at(i);
        }
        send[2 + mes.size()] = 0;
        ch->sendBytes(send, 2 + mes.size() + 1);
    } else if (op == "DISC") {
        char send[2] = {0, 10};
        ch->sendBytes(send, 2);
    }
}

void Protocol::process(Message *message) {
    short opCode = message->opcode;
    switch (opCode) {

        case 3: {
            DataMessage *dm = (DataMessage *) message;
            if (receiveNextPacket(dm)) {//if was the last packet
                if (currentState == ReceivingFile) {
                    ofstream myfile;
                    myfile.open(filename.c_str());
                    myfile.write(readBuffer.data(), bytesReceived);
                    myfile.close();


                    std::cout << "RRQ " << filename << " complete" << std::endl;
                    filename = "";
                } else if (currentState == Dirq) {
                    vector<std::string> files;
                    string file;
                    for (unsigned int i = 0; i < readBuffer.size(); ++i) {
                        if (readBuffer.at(i) != '\0') {
                            file += readBuffer.at(i);
                        } else {
                            files.push_back(file);
                            file.clear();
                        }
                    }

                    for (unsigned int j = 0; j < files.size(); ++j) {
                        std::cout << files.at(j) << std::endl;
                    }
                }


                bytesReceived = 0;
                readBuffer.clear();
                currentState = 0; //Waiting
            } else {

            }
            break;
        }
        case 4: {
            ackReceived((AckMessage *) message);
            break;
        }
        case 5: {
            ErrMessage *em = (ErrMessage *) message;
            std::cout << "Error " << em->errCode << " - " << em->msg << std::endl;
            delete writeBuffer;

            bytesRemaining = 0;
            bytesSent = 0;
            bytesReceived = 0;
            counter =0;
            currentState = Waiting;
            filename.clear();
            blockNum = 0;
            packetSize = 0;
            readerArr.clear();
            break;
        }
        case 9: {
            BcastMessage *bcm = (BcastMessage *) message;
            std::cout << "BCAST " << (bcm->added ? "add " : "del ") << bcm->filename << std::endl;
            break;
        }
    }
}

bool Protocol::receiveNextPacket(DataMessage *dm) {
    bool ret = false;
//    if (dm->size != 0) {
        for (int i = 0; i < dm->size; ++i) {
            readBuffer.push_back(dm->bArr[i]);
        }

        // send ack
        char send[4];
        send[0] = 0;
        send[1] = 4;
        send[2] = (dm->block >> 8) & 0xFF;
        send[3] = (dm->block & 0xFF);
        ch->sendBytes(send, 4);
//    }
    if (dm->size < 512) {
        ret = true;
    }
    bytesReceived += dm->size;
    return ret;
}

bool Protocol::sendNextPacket() {
    bool ret = false;
    if (bytesRemaining <= 512 && bytesRemaining != 0) {
        char a[bytesRemaining];
        for (int i = 0; i < bytesRemaining; ++i) {
            a[i] = writeBuffer[bytesSent + i];
        }
        DataMessage dataMessage(bytesRemaining, blockNum, a);

        short byteSize = 6 + dataMessage.size;
        char b[byteSize];
        b[0] = 0;
        b[1] = 3;
        b[2] = (dataMessage.size >> 8) & 0xFF;
        b[3] = (dataMessage.size & 0xFF);
        b[4] = (dataMessage.block >> 8) & 0xFF;
        b[5] = (dataMessage.block & 0xFF);
        for (int i = 0; i < dataMessage.size; ++i) {
            b[6 + i] = dataMessage.bArr[i];
        }

        ch->sendBytes(b, byteSize);
        bytesRemaining = 0;
        bytesSent += bytesRemaining;
        delete writeBuffer;
        ret = true;
    } else if (bytesRemaining > 512) {
        char a[512];
        for (int i = 0; i < 512; ++i) {
            a[i] = writeBuffer[bytesSent + i];
        }
        DataMessage dataMessage(512, blockNum, a);
        short byteSize = 6 + dataMessage.size;
        char b[byteSize];
        b[0] = 0;
        b[1] = 3;
        b[2] = (dataMessage.size >> 8) & 0xFF;
        b[3] = (dataMessage.size & 0xFF);
        b[4] = (dataMessage.block >> 8) & 0xFF;
        b[5] = (dataMessage.block & 0xFF);
        for (int i = 0; i < dataMessage.size; ++i) {
            b[6 + i] = dataMessage.bArr[i];
        }

        ch->sendBytes(b, byteSize);
        bytesRemaining -= 512;
        bytesSent += 512;
        ret = true;
    } else if (bytesRemaining == 0 && sendZeroBits) {
        char a[0];
        DataMessage dataMessage(0, blockNum, a);

        short byteSize = 6 + dataMessage.size;
        char b[byteSize];
        b[0] = 0;
        b[1] = 3;
        b[2] = (dataMessage.size >> 8) & 0xFF;
        b[3] = (dataMessage.size & 0xFF);
        b[4] = (dataMessage.block >> 8) & 0xFF;
        b[5] = (dataMessage.block & 0xFF);
        for (int i = 0; i < dataMessage.size; ++i) {
            b[6 + i] = dataMessage.bArr[i];
        }

        ch->sendBytes(b, byteSize);

        sendZeroBits = false;
        ret = true;
    }
    blockNum++;
    return ret;
}


bool Protocol::ackReceived(AckMessage *m) {
    std::cout << "ACK " << m->block << std::endl;
    switch (currentState) {
        case 1: //SendingFile
            if (!sendNextPacket()) {
                currentState = 0; //Waiting
                cout << "WRQ " << filename << " complete" << endl;
            }
            break;
        default:
            break;
    }

    return true;
}

short Protocol::bytesToShort(char *bytesArr) {
    short result = (short) ((bytesArr[0] & 0xff) << 8);
    result += (short) (bytesArr[1] & 0xff);
    return result;
}

void Protocol::shortToBytes(short num, char *bytesArr) {
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}


