#pragma once
#include <stdint.h>
#include "NecMessage.h"

#define NEC_PRINT_BYTES
#define NEC_PRINT_BITS

enum NecDelay : uint32_t {
    NecPreambleMark = 90000,
    NecPreambleSpace = 45000,
    NecBitMark = 560,
    NecOneSpace = 1690,
    NecZeroSpace = 560,
    NecRepeatSpace = 22500,
    NecPostambleEnd = 400000,
    NecRepeatEnd = 900000,
    NecEndLow = 0
};

namespace NecConstants {
    static const bool kDefaultMarkOutputLevel = LOW;
    static const uint8_t kMessageLengthInBytes = 4;
    static const uint8_t kDefaultOutputPin = 5;
}

class NecInterface {
    public:
        NecInterface();
        NecInterface(bool markOutputLevel, uint8_t outputPin);

        void setOutputPin(uint8_t outputPin);
        void sendNec(uint8_t messageBytes[4]);
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
