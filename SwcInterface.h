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
// []

namespace SwcConstants{
    static const u8 kDefaultSwcInputPin = 0;
};

class SwcInterface {
    public:
        SwcInterface();
        SwcInterface(u8 inputPin);

        SwcButton readSwc();
#ifdef DEBUG_SWC
        void printSwc(SwcButton buttonPressed);
#endif

    private:
        _inputPin = 0;
#ifdef DEBUG_SWC
        SwcButton _lastSwcButtonPressed = SwcEndOfEnum;
#endif
}
