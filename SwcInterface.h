#pragma once
#include <stdint.h>
#include "SwcButtonEnum.h"

// Raw Analog Pin Read Values
// for 2007 Mazda 5 Steering Wheel Controls
// using Voltage Divider
// [VolUp, VolDown, SeekUp, SeekDown, Mute, Mode, Open]
// w/ 10k Ohm Resistor
// [3, 13, 28, 53, 95, 173, 344]
// w/ 4.7k Ohm Resistor
// [10, 30, 61, 110, 186, 312, 533]

namespace SwcConstants{
    static const uint8_t kDefaultSwcInputPin = 0;
    static const uint8_t kDefaultSwcVerificationDelay = 1;
};

class SwcInterface {
    public:
        SwcInterface();
        SwcInterface(uint8_t inputPin);

        SwcButton readSwc();
        void printSwc(SwcButton buttonPressed);

    private:
        uint8_t _inputPin = 0x00;
};
