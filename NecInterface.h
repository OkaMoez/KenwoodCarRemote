#pragma once
#include <Arduino.h>
#include "NecQueue.h"

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
        bool isReadyForNewMessage();
        void sendNecMessage(NecMessage necMessage);
        void sendNecMessage(uint32_t message);
        void sendNecRepeat();

    private:
        bool _markOutputLevel = LOW;
        uint8_t _outputPin = 0x00;

        NecQueue _queue;
        NecPulse _currentPulse = NecPulse::None;
        unsigned long _pulseStart = 0;
        unsigned long _pulseDelay = 0;

        bool _isReadyForNextPulse();
        bool _checkTimeAndWait(unsigned long timer);

        void _sendNextNecPulse();

        void _sendNecOne();
        void _sendNecZero();

        void _beginWritingNecMark(NecDelay delay);
        void _beginWritingNecSpace(NecDelay delay);
        void _beginWritingNec(bool outputLevel, NecDelay delay);
        
        void _continueNecPulse();

        void _writeNecMark(NecDelay delay);
        void _writeNecSpace(NecDelay delay);
        void _writeNec(bool outputLevel, NecDelay delay);
};
