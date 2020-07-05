#include "NecInterface.h"

#include "LocalDefine.h"
#include "NecMessage.h"
#include "NecPulse.h"

NecInterface::NecInterface() : 
    NecInterface(
        NecConstants::kDefaultMarkOutputLevel, 
        NecConstants::kDefaultOutputPin) {
}

NecInterface::NecInterface(bool markOutputLevel, uint8_t outputPin) :
    _markOutputLevel(markOutputLevel),
    _outputPin(outputPin) {
    pinMode(_outputPin, OUTPUT);
#ifdef DEBUG
    String textOut = "[NEC Interface] Initialized";
    textOut += "\n[NEC Interface] Mark Level: " + String(_markOutputLevel);
    textOut += "\n[NEC Interface] Output Pin: " + String(_outputPin);    
    Serial.println(textOut);
#endif
}

void NecInterface::tick() {
    if ((micros() - _pulseStart) >= _pulseDelay) {
        _sendNextNecPulse();
    }
}

void NecInterface::setOutputPin(uint8_t pin) {
    _outputPin = pin;
}

bool NecInterface::readyForNewMessage() {
    return _queue.empty();
}

void NecInterface::sendNecMessage(NecMessage necMessage) {
    uint32_t message = 0x00000000;

    message ^= uint32_t(necMessage.mMessage[0]) << 24;
    message ^= uint32_t(necMessage.mMessage[1]) << 16;
    message ^= uint32_t(necMessage.mMessage[2]) << 8;
    message ^= uint32_t(necMessage.mMessage[3]);

#ifdef DEBUG_NEC_BYTES
        String textOut = "[NEC Interface] Assembled Message: " + String(message);
        Serial.println(textOut);
#endif

    sendNecMessage(message);
}

void NecInterface::sendNecMessage(uint32_t rawMessage) {
#ifdef DEBUG_NEC_BITS
    Serial.println("[NEC Interface] Sending Message...");
    int counter = 1;
#endif

    _enqueueNecPreamble();
    for (uint32_t  mask = uint32_t(1);  mask != uint32_t(0);  mask <<= 1) {
        if (rawMessage & mask) {
            _enqueueNecOne();
        } else {
            _enqueueNecZero();
        }
#ifdef DEBUG_NEC_BITS
        if ((counter % 8) == 0) {
            Serial.print(" ");
        }
        counter++;
#endif
    }
    _enqueueNecPostamble();
}

void NecInterface::sendNecRepeat() {
    _enqueueNecMark(NecDelay::PreambleMark);
    _enqueueNecSpace(NecDelay::RepeatSpace);
    _enqueueNecMark(NecDelay::BitMark);
    _enqueueNecSpace(NecDelay::RepeatEnd);

#ifdef DEBUG
    Serial.println("[NEC Interface] Repeat Sent");
#endif
}

void NecInterface::_enqueueNecPreamble() {
    _enqueueNecMark(NecDelay::PreambleMark);
    _enqueueNecSpace(NecDelay::PreambleSpace);
    
#ifdef DEBUG
    Serial.print("[NEC Interface] Preamble/ ");
#endif
}

void NecInterface::_enqueueNecPostamble() {
    _enqueueNecMark(NecDelay::BitMark);
    _enqueueNecSpace(NecDelay::PostambleEnd);
    
#ifdef DEBUG_NEC_BITS
    Serial.println("/Postamble");
#endif
}

void NecInterface::_enqueueNecOne() {
    _enqueueNecMark(NecDelay::BitMark);
    _enqueueNecSpace(NecDelay::OneSpace);

#ifdef DEBUG_NEC_BITS
    Serial.print("1");
#endif
}

void NecInterface::_enqueueNecZero() {
    _enqueueNecMark(NecDelay::BitMark);
    _enqueueNecSpace(NecDelay::ZeroSpace);
    
#ifdef DEBUG_NEC_BITS
    Serial.print("0");
#endif
}

void NecInterface::_enqueueNecMark(NecDelay delay) {
    _queue.enqueue(NecPulse(_markOutputLevel, static_cast<uint32_t>(delay)));
}

void NecInterface::_enqueueNecSpace(NecDelay delay) {
    _queue.enqueue(NecPulse(!_markOutputLevel, static_cast<uint32_t>(delay)));
}

void NecInterface::_sendNextNecPulse() {
    if (_queue.empty()) {
        return;
    }

    NecPulse pulse = _queue.dequeue();
    digitalWrite(_outputPin, pulse.mLevel);
    _pulseStart = micros();
    _pulseDelay = pulse.mDelay;

#ifdef DEBUG
    String textOut = "[NEC Interface] New Bit";    
    Serial.println(textOut);
#endif
}
