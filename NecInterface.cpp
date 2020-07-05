#include <Arduino.h>
#include "LocalDefine.h"
#include "NecInterface.h"
#include "NecMessage.h"

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
    String textOut = "[NEC Interface] Initialized\n";
    textOut += "[NEC Interface] Mark Level: " + String(_markOutputLevel) + "\n";
    textOut += "[NEC Interface] Output Pin: " + String(_outputPin);    
    Serial.println(textOut);
#endif
}

void NecInterface::setOutputPin(uint8_t pin) {
    _outputPin = pin;
}

void NecInterface::sendNec(NecMessage necMessage) {
    uint32_t message = 0x00000000;

    message ^= uint32_t(necMessage.mMessage[0]) << 24;
    message ^= uint32_t(necMessage.mMessage[1]) << 16;
    message ^= uint32_t(necMessage.mMessage[2]) << 8;
    message ^= uint32_t(necMessage.mMessage[3]);

#ifdef DEBUG_NEC_BYTES
        Serial.print("Assembled Message: ");
        Serial.println(message);
#endif

    sendNec(message);
}

void NecInterface::sendNec(uint32_t rawMessage) {
#ifdef DEBUG_NEC_BITS
    Serial.print(rawMessage);
    Serial.print(" ");
    int counter = 1;
#endif

    _sendNecPreamble();
    for (uint32_t  mask = uint32_t(1);  mask == (uint32_t(1) << (32 - 1));  mask <<= 1) {
        if (rawMessage & mask) {
            _sendNecOne();
        } else {
            _sendNecZero();
        }
#ifdef DEBUG_NEC_BITS
        if ((counter % 8) == 0) {
            Serial.print(" ");
        }
        counter++;
#endif
    }
    _sendNecPostamble();
}

void NecInterface::sendNecRepeat() {
    _writeNecMark(NecPreambleMark);
    _writeNecSpace(NecRepeatSpace);
    _writeNecMark(NecBitMark);
    _writeNecSpace(NecRepeatEnd);
}

void NecInterface::_sendNecPreamble() {
    _writeNecMark(NecPreambleMark);
    _writeNecSpace(NecPreambleSpace);
}

void NecInterface::_sendNecPostamble() {
    _writeNecMark(NecBitMark);
    _writeNecSpace(NecPostambleEnd);
    
#ifdef DEBUG_NEC_BITS
    Serial.println();
#endif
}

void NecInterface::_sendNecOne() {
    _writeNecMark(NecBitMark);
    _writeNecSpace(NecOneSpace);

#ifdef DEBUG_NEC_BITS
    Serial.print("1");
#endif
}

void NecInterface::_sendNecZero() {
    _writeNecMark(NecBitMark);
    _writeNecSpace(NecZeroSpace);
    
#ifdef DEBUG_NEC_BITS
    Serial.print("0");
#endif
}

void NecInterface::_writeNecMark(NecDelay delay) {
    digitalWrite(_outputPin, _markOutputLevel);
    delayMicroseconds(delay);
}

void NecInterface::_writeNecSpace(NecDelay delay) {
    digitalWrite(_outputPin, !_markOutputLevel);
    delayMicroseconds(delay);
}
