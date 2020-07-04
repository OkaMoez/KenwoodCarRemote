#include "KenwoodNecInterface.h"

KenwoodNecInterface::KenwoodNecInterface() : 
    KenwoodNecInterface(
        NecConstants::kDefaultMarkOutputLevel, 
        KenwoodConstants::kDefaultOutputPin, 
        KenwoodConstants::kDefaultDeviceAddress) {
}

KenwoodNecInterface::KenwoodNecInterface(bool markOutputLevel, u8 outputPin, u8 deviceAddress) {
    _markOutputLevel = markOutputLevel;
    _outputPin = outputPin;
    _deviceAddress = deviceAddress;
    pinMode(_outputPin, OUTPUT);
}

void KenwoodNecInterface::setOutputPin(u8 pin) {
    _outputPin = pin;
}

void KenwoodNecInterface::setKenwoodAddress(u8 address) {
    _deviceAddress = address;
}

void KenwoodNecInterface::sendSwcToKenwood(SwcButton button) {
#ifdef DEBUG_NEC
    Serial.print("Button Code Recieved: ");
    Serial.println(button);
#endif
    if (button == _lastSwcButton) {
        _sendKenwoodRepeat();
#ifdef DEBUG_NEC
        Serial.print("Repeat Code: ");
        Serial.println(button);
#endif
        return;
    }

    _sendKenwoodCommand(KenwoodConstants::kDefaultCodes[button]);
    _lastSwcButton = button;
}

void KenwoodNecInterface::clearLastSwcButtonToKenwood() {
    _lastSwcButton = SwcEndOfEnum;
}

#ifdef DEBUG_NEC
void KenwoodNecInterface::debugSendKenwoodCommand(u8 command) {
    _sendKenwoodCommand(command);
}

void KenwoodNecInterface::debugSendNec(u32 message) {
    _sendNec(message);
}
#endif

void KenwoodNecInterface::_sendKenwoodCommand(u8 command) {
    u8 messageBytes[4];

    messageBytes[0] = _deviceAddress;
    messageBytes[1] = ~_deviceAddress;
    messageBytes[2] = command;
    messageBytes[3] = ~command;

#ifdef VERBOSE_BITS
    Serial.print("Array Contents: ");
    Serial.print(messageBytes[0]);
    Serial.print(" ");
    Serial.print(messageBytes[1]);
    Serial.print(" ");
    Serial.print(messageBytes[2]);
    Serial.print(" ");
    Serial.println(messageBytes[3]);
#endif

#ifdef LSBIT_FIRST
    u8 buffer = 0;
    for (int i = 0; i < 4; i++) {
        u8 mask = u8(1);
        for (u8 j = 0; j < 8; j++) {
            buffer <<= 1;
            buffer ^= (messageBytes[i] & mask);
            messageBytes[i] >>= 1;
        }
        messageBytes[i] = buffer;
        buffer = 0;
    }
#ifdef VERBOSE_BITS
    Serial.print("Reversed Contents: ");
    Serial.print(messageBytes[0]);
    Serial.print(" ");
    Serial.print(messageBytes[1]);
    Serial.print(" ");
    Serial.print(messageBytes[2]);
    Serial.print(" ");
    Serial.println(messageBytes[3]);
#endif
#endif

#ifdef LSBYTE_FIRST
    messageBytes[3] = _deviceAddress;
    messageBytes[2] = ~_deviceAddress;
    messageBytes[1] = command;
    messageBytes[0] = ~command;
#endif

    // Message will be encoded with Most Significant Byte first
    _sendNec(messageBytes);
}

void KenwoodNecInterface::_sendKenwoodRepeat() {
    _sendNecRepeat();
}

void KenwoodNecInterface::_sendNec(u8 messageBytes[4]) {
    u32 message = 0x00000000;

    for (u8 i = 0; i < 4; i++) {
        message <<= 8;
        message ^= messageBytes[i];
    }
#ifdef VERBOSE_BITS
        Serial.print("Assembled Message: ");
        Serial.println(message);
#endif

    _sendNec(message);
}

void KenwoodNecInterface::_sendNec(u32 message) {
    _sendNecPreamble();

#ifdef VERBOSE_BITS
    Serial.print(message);
    Serial.print(" ");
    int counter = 1;
#endif
    for (u32  mask = u32(1) << (32 - 1);  mask;  mask >>= 1) {
        if (message & mask) {
            _sendNecOne();
#ifdef VERBOSE_BITS
            Serial.print("1");
#endif
        } else {
            _sendNecZero();
#ifdef VERBOSE_BITS
            Serial.print("0");
#endif
        }
#ifdef VERBOSE_BITS
        if ((counter % 8) == 0) {
            Serial.print(" ");
        }
        counter++;
#endif
    }
#ifdef VERBOSE_BITS
    Serial.println();
#endif

    _sendNecPostamble();
}

void KenwoodNecInterface::_sendNecRepeat() {
    _sendNecMark(NecPreambleMark);
    _sendNecSpace(NecRepeatSpace);
    _sendNecMark(NecBitMark);
    _sendNecSpace(NecRepeatEnd);
}

void KenwoodNecInterface::_sendNecPreamble() {
    _sendNecMark(NecPreambleMark);
    _sendNecSpace(NecPreambleSpace);
}

void KenwoodNecInterface::_sendNecPostamble() {
    _sendNecMark(NecBitMark);
    _sendNecSpace(NecPostambleEnd);
}

void KenwoodNecInterface::_sendNecOne() {
    _sendNecMark(NecBitMark);
    _sendNecSpace(NecOneSpace);
}

void KenwoodNecInterface::_sendNecZero() {
    _sendNecMark(NecBitMark);
    _sendNecSpace(NecZeroSpace);
}

void KenwoodNecInterface::_sendNecMark(NecDelay delay) {
    digitalWrite(_outputPin, _markOutputLevel);
    delayMicroseconds(delay);
}

void KenwoodNecInterface::_sendNecSpace(NecDelay delay) {
    digitalWrite(_outputPin, !_markOutputLevel);
    delayMicroseconds(delay);
}
