//
// Created by yonatan on 1/11/17.
//

#ifndef A3_SPL_CLIENT_ENCODERDECODER_H
#define A3_SPL_CLIENT_ENCODERDECODER_H


#include "Message.h"

class EncoderDecoder {
private:

public:
    EncoderDecoder();

    Message decodeNextByte(char nextByte);


    char* encode(Message message);
};


#endif //A3_SPL_CLIENT_ENCODERDECODER_H
