#pragma once
#include <Arduino.h>

struct NecPulse {
    bool mLevel = LOW;
    uint32_t mDelay = 0;

    NecPulse() {}

    NecPulse(bool level, uint32_t delay) : 
        mLevel(level), 
        mDelay(delay) {}
};