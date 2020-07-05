#pragma once
#include <stdint.h>

enum class SwcButton : uint8_t {
    VolumeUp = 0,
    VolumeDown = 1,
    SeekUp = 2,
    SeekDown = 3,
    Mode = 4,
    Mute = 5,
    EndOfEnum = 6,
    Open = 15,
    Error = 16
};
