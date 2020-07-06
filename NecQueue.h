#pragma once
#include <stdint.h>
#include "NecPulse.h"

namespace NecQueueConstants {
    static const uint8_t kMaxQueueSize = 34;
}

class NecQueue {
    public:
        NecQueue();

        bool full();
        bool empty();
        uint8_t size();

        void enqueue(NecPulse pulse);
        NecPulse dequeue();

    private:
        NecPulse _queue[NecQueueConstants::kMaxQueueSize];
        uint8_t _size = 0;
        uint8_t _front = 0;
        uint8_t _rear = 0;
};