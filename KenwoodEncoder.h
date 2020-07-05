#pragma once
#include <stdint.h>
#include "SwcButtonEnum.h"

//#define VERBOSE_BITS
#define DEBUG_NEC
#define LSBIT_FIRST
//#define LSBYTE_FIRST

namespace KenwoodConstants {
    static const uint8_t kDefaultDeviceAddress = 0xb9;

    // Kenwood Remote Codes
    static const uint8_t kDefaultCodes[6] = {
        0x14, // VolumeUp
        0x15, // VolumeDown
        0x0b, // SeekUp
        0x0a, // SeekDown
        0x13, // Mode
        0x16 // Mute
    };
}

struct NecMessage;

class KenwoodEncoder {
    public:
        KenwoodEncoder();
        KenwoodEncoder(uint8_t deviceAddress);

        void setKenwoodDeviceAddress(uint8_t deviceAddress);
        NecMessage buildNecMessage(SwcButton button);
        NecMessage buildNecMessage(uint8_t kenwoodCode);

    private:
        uint8_t _deviceAddress = 0x00;
};
