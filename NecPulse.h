#pragma once
#include <Arduino.h>

enum class NecPulse : uint8_t {
    Zero = 0,
    One = 1,
    Preamble = 2,
    PreambleEnd = 3,
    Postamble = 4,
    PostambleEnd = 5,
    Repeat = 6,
    RepeatTwo = 7,
    RepeatThree = 8,
    RepeatEnd = 9,
    None = 15
};