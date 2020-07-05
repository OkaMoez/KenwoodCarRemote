#pragma once
#include <stdint.h>

struct NecMessage {
    uint8_t mMessage[4] = { 0x00, 0x00, 0x00, 0x00};

    NecMessage(uint8_t message[4]) {
        for (uint8_t i = 0; i < 4; i++) {
            mMessage[i] = message[i];
        }
    }
};
