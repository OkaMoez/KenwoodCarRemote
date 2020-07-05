#pragma once
#include <stdint.h>
#include "NecMessage.h"

#define NEC_PRINT_BYTES
#define NEC_PRINT_BITS

enum class NecDelay : uint32_t {
    // In microseconds
    PreambleMark = 9000,
    PreambleSpace = 4500,
    BitMark = 560,
    OneSpace = 1690,
    ZeroSpace = 560,
    RepeatSpace = 2250,
    PostambleEnd = 40000,
    RepeatEnd = 90000,
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
