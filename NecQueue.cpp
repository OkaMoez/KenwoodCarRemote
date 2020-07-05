#include "NecQueue.h"

NecQueue::NecQueue() {

}

bool NecQueue::full() {
    return _size == NecQueueConstants::kMaxQueueSize;
}

bool NecQueue::empty() {
    return _size == 0;
}

uint8_t NecQueue::size() {
    return _size;
}

void NecQueue::enqueue(NecPulse pulse) {
    if (full()) {
#ifdef DEBUG
        Serial.println("[Nec Interface] ERROR: QUEUE OVERFLOW");
#endif
        return;
    }

    _rear = (_rear + 1) % NecQueueConstants::kMaxQueueSize;
    _queue[_rear] = pulse;
    _size++;
}

NecPulse NecQueue::dequeue() {
    if (empty()) {
#ifdef DEBUG
        Serial.println("[Nec Interface] ERROR: Atempting to Read Empty Queue");
#endif
        return;
    }

    uint8_t tempFront = _front;
    _front = (_front + 1) % NecQueueConstants::kMaxQueueSize;
    _size--;
    return _queue[tempFront];
}
