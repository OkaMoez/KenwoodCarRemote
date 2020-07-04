#pragma once
#include <stdint.h>
#include "SwcButtonEnum.h"

//#define VERBOSE_BITS
#define DEBUG_NEC
#define LSBIT_FIRST
//#define LSBYTE_FIRST

enum NecDelay : u32 {
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

namespace NecConstants{
    static const bool kDefaultMarkOutputLevel = LOW;
    static const u8 kMessageLengthInBytes = 4;
}

namespace KenwoodConstants {
    static const u8 kDefaultDeviceAddress = 0xb9;
    static const u8 kDefaultOutputPin = 5;

    // Kenwood Remote Codes
    static const u8 kDefaultCodes[6] = {
        0x14, // VolumeUp
        0x15, // VolumeDown
        0x0b, // SeekUp
        0x0a, // SeekDown
        0x13, // Mode
        0x16 // Mute
    };
}

class KenwoodNecInterface {
    public:
        KenwoodNecInterface();
        KenwoodNecInterface(bool markOutputLevel, u8 outputPin, u8 deviceAddress);

        void setOutputPin(u8 outputPin);
        void setKenwoodAddress(u8 deviceAddress);
        void sendSwcToKenwood(SwcButton button);
        void clearLastSwcButtonToKenwood();

#ifdef DEBUG_NEC
        void debugSendKenwoodCommand(u8 command);
        void debugSendNec(u32 message);
#endif

    private:
        bool _markOutputLevel = LOW;
        u8 _outputPin = 0x00;
        u8 _deviceAddress = 0x00;
        SwcButton _lastSwcButton = SwcEndOfEnum;

        void _sendKenwoodCommand(u8 command);
        void _sendKenwoodRepeat();

        void _sendNec(u8 messageBytes[4]);
        void _sendNec(u32 message);
        void _sendNecRepeat();
        void _sendNecPreamble();
        void _sendNecPostamble();

        void _sendNecOne();
        void _sendNecZero();

        void _sendNecMark(NecDelay delay);
        void _sendNecSpace(NecDelay delay);
};
