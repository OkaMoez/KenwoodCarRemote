#pragma once
#include <stdint.h>

enum SwcButton : u8 {
    SwcVolumeUp = 0,
    SwcVolumeDown = 1,
    SwcSeekUp = 2,
    SwcSeekDown = 3,
    SwcMode = 4,
    SwcMute = 5,
    SwcEndOfEnum = 6,
    SwcOpen = 15,
    SwcError = 16
};