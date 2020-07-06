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
    unsigned long pulseTimer = micros() - _pulseStart;

    if (isReadyForNewMessage()) {
        return;
    }

#ifdef DEBUG_NEC_DELAY
    String textOut = "[NEC Interface] Tick Delay: " + String(pulseTimer);  
    Serial.println(textOut);
#endif

    if (!_isReadyForNextPulse() && _checkTimeAndWait(pulseTimer)) {
        _continueNecPulse();
    }

    while (_isReadyForNextPulse() && !isReadyForNewMessage()) {
        _sendNextNecPulse();
    }
}

void NecInterface::setOutputPin(uint8_t pin) {
    _outputPin = pin;
}

bool NecInterface::isReadyForNewMessage() {
    return (_queue.empty() && (_currentPulse == NecPulse::None));
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

    _queue.enqueue(NecPulse::Preamble);
#ifdef DEBUG_NEC_BITS
            Serial.print("[NEC Interface] Preamble/ ");
#endif
    for (uint32_t  mask = uint32_t(1);  mask != uint32_t(0);  mask <<= 1) {
        if (rawMessage & mask) {
            _queue.enqueue(NecPulse::One);
#ifdef DEBUG_NEC_BITS
            Serial.print("1");
#endif
        } else {
            _queue.enqueue(NecPulse::Zero);
#ifdef DEBUG_NEC_BITS
            Serial.print("0");
#endif
        }
#ifdef DEBUG_NEC_BITS
        if ((counter % 8) == 0) {
            Serial.print(" ");
        }
        counter++;
#endif
    }
    _queue.enqueue(NecPulse::Postamble);
#ifdef DEBUG_NEC_BITS
            Serial.println("/Postamble");
#endif
}

void NecInterface::sendNecRepeat() {
    _queue.enqueue(NecPulse::Repeat);
}

bool NecInterface::_isReadyForNextPulse() {
    return _currentPulse == NecPulse::None;
}

bool NecInterface::_checkTimeAndWait(unsigned long timer) {
    if (timer >= _pulseDelay) {
    }
    else if ((_pulseDelay - timer) <= 2600) {
        delayMicroseconds((_pulseDelay - timer) - 200);
    }
    else {
        return false;
    }
    return true;
}

void NecInterface::_sendNextNecPulse() {
    if (_queue.empty()) {
        return;
    }

#ifdef DEBUG
    String textOut = "[NEC Interface] Next Pulse - ";
#endif
    _currentPulse = _queue.dequeue();

    switch (_currentPulse) {
        case NecPulse::Preamble:
            _beginWritingNecMark(NecDelay::PreambleMark);
#ifdef DEBUG
            textOut += "Preamble";
#endif
            break;
        case NecPulse::Zero:
            _sendNecZero();
            _currentPulse = NecPulse::None;
#ifdef DEBUG
            textOut += "Zero";
#endif
            break;
        case NecPulse::One:
            _sendNecOne();
            _currentPulse = NecPulse::None;
#ifdef DEBUG
            textOut += "One";
#endif
            break;
        case NecPulse::Postamble:
            _writeNecMark(NecDelay::BitMark);
            _beginWritingNecSpace(NecDelay::PreambleMark);
#ifdef DEBUG
            textOut += "Postamble";
#endif
            break;
        case NecPulse::Repeat:
            _beginWritingNecMark(NecDelay::PreambleMark);
#ifdef DEBUG
            textOut += "Repeat";
#endif
            break;
        default:
#ifdef DEBUG
            Serial.println("[NEC Interface] ERROR: Attempting to Send Empty Pulse");
#endif
            return;
    }

#ifdef DEBUG
    Serial.println(textOut);
#endif
}

void NecInterface::_sendNecOne() {
    _writeNecMark(NecDelay::BitMark);
    _writeNecSpace(NecDelay::OneSpace);
}

void NecInterface::_sendNecZero() {
    _writeNecMark(NecDelay::BitMark);
    _writeNecSpace(NecDelay::ZeroSpace);
}

void NecInterface::_beginWritingNecMark(NecDelay delay) {
    _beginWritingNec(_markOutputLevel, delay);
}

void NecInterface::_beginWritingNecSpace(NecDelay delay) {
    _beginWritingNec(!_markOutputLevel, delay);
}

void NecInterface::_beginWritingNec(bool outputLevel, NecDelay delay) {
    digitalWrite(_outputPin, outputLevel);
    _pulseStart = micros();
    _pulseDelay = static_cast<uint32_t>(delay);
}

void NecInterface::_continueNecPulse() {
#ifdef DEBUG
    unsigned long pulseTimer = micros() - _pulseStart;
    String textOut = "[NEC Interface] Final Command Execution Time: " + String(pulseTimer);
    textOut += "\n[NEC Interface] Continue Pulse - ";
#endif
    switch (_currentPulse) {
        case NecPulse::Preamble:
            _beginWritingNecSpace(NecDelay::PreambleSpace);
            _currentPulse = NecPulse::PreambleEnd;
#ifdef DEBUG
            textOut += "Preamble [1/2]";
#endif
            break;
        case NecPulse::PreambleEnd:
            _currentPulse = NecPulse::None;
#ifdef DEBUG
            textOut += "Preamble [2/2]";
#endif
            break;
        case NecPulse::Postamble:
            _beginWritingNecSpace(NecDelay::PostambleEnd);
            _currentPulse = NecPulse::PostambleEnd;
#ifdef DEBUG
            textOut += "Postamble [1/2]";
#endif
            break;
        case NecPulse::PostambleEnd:
            _currentPulse = NecPulse::None;
#ifdef DEBUG
            textOut += "Postamble [2/2]";
#endif
            break;
        case NecPulse::Repeat:
            _beginWritingNecSpace(NecDelay::RepeatSpace);
            _currentPulse = NecPulse::RepeatTwo;
#ifdef DEBUG
            textOut += "Repeat [1/4]";
#endif
            break;
        case NecPulse::RepeatTwo:
            _beginWritingNecMark(NecDelay::BitMark);
            _currentPulse = NecPulse::RepeatThree;
#ifdef DEBUG
            textOut += "Repeat [2/4]";
#endif
            break;
        case NecPulse::RepeatThree:
            _beginWritingNecSpace(NecDelay::RepeatEnd);
            _currentPulse = NecPulse::RepeatEnd;
#ifdef DEBUG
            textOut += "Repeat [3/4]";
#endif
            break;
        case NecPulse::RepeatEnd:
            _currentPulse = NecPulse::None;
#ifdef DEBUG
            textOut += "Repeat [4/4]";
#endif
            break;
        default:
#ifdef DEBUG
            Serial.println("[NEC Interface] ERROR: Attempting to Continue Unknown Pulse");
            Serial.println("[NEC Interface] " + String(static_cast<int>(_currentPulse)));
#endif
            return;
    }

#ifdef DEBUG
    Serial.println(textOut);
#endif
}

void NecInterface::_writeNecMark(NecDelay delay) {
    _writeNec(_markOutputLevel, delay);
}

void NecInterface::_writeNecSpace(NecDelay delay) {
    _writeNec(!_markOutputLevel, delay);
}

void NecInterface::_writeNec(bool outputLevel, NecDelay delay) {
#ifdef DEBUG
    if (static_cast<uint32_t>(delay) >= 3000) {
        String textOut = "[NEC Interface] Syncronous write function is not suited for long delays. ";
        textOut += "Delay: " + String(static_cast<uint32_t>(delay));
        Serial.println();
    }
#endif
    digitalWrite(_outputPin, outputLevel);
    delayMicroseconds(static_cast<uint32_t>(delay));
}
