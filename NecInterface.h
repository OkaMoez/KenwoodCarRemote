#pragma once
#include <stdint.h>
#include "NecMessage.h"

#define NEC_PRINT_BYTES
#define NEC_PRINT_BITS

enum class NecDelay : uint32_t {
    PreambleMark = 90000,
    PreambleSpace = 45000,
    BitMark = 560,
    OneSpace = 1690,
    ZeroSpace = 560,
    RepeatSpace = 22500,
    PostambleEnd = 400000,
    RepeatEnd = 900000,
    EndLow = 0
};

namespace NecConstants {
    static const bool kDefaultMarkOutputLevel = LOW;
    static const uint8_t kMessageLengthInBytes = 4;
    static const uint8_t kDefaultOutputPin = 5;
}

struct NecMessage;

class NecInterface {
    public:
        NecInterface();
        NecInterface(bool markOutputLevel, uint8_t outputPin);

        void setOutputPin(uint8_t outputPin);
        void sendNec(NecMessage necMessage);
        void sendNec(uint32_t message);
        void sendNecRepeat();

    private:
        bool _markOutputLevel = LOW;
        uint8_t _outputPin = 0x00;

        void _sendNecPreamble();
        void _sendNecPostamble();

        void _sendNecOne();
        void _sendNecZero();

        void _writeNecMark(NecDelay delay);
        void _writeNecSpace(NecDelay delay);
};
