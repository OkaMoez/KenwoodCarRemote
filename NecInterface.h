#pragma once
#include <Arduino.h>
#include "NecQueue.h"

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

        void tick();

        void setOutputPin(uint8_t outputPin);
        bool readyForNewMessage();
        void sendNecMessage(NecMessage necMessage);
        void sendNecMessage(uint32_t message);
        void sendNecRepeat();

    private:
        NecQueue _queue;
        bool _markOutputLevel = LOW;
        uint8_t _outputPin = 0x00;
        unsigned long _pulseStart = 0;
        unsigned long _pulseDelay = 0;

        void _enqueueNecPreamble();
        void _enqueueNecPostamble();

        void _enqueueNecOne();
        void _enqueueNecZero();

        void _enqueueNecMark(NecDelay pulse);
        void _enqueueNecSpace(NecDelay pulse);

        void _sendNextNecPulse();
};
