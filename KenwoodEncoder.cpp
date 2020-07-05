#include <Arduino.h>
#include "LocalDefine.h"
#include "KenwoodEncoder.h"
#include "NecMessage.h"

KenwoodEncoder::KenwoodEncoder() : 
    KenwoodEncoder(
        KenwoodConstants::kDefaultDeviceAddress) {
}

KenwoodEncoder::KenwoodEncoder(uint8_t deviceAddress) :
    _deviceAddress(deviceAddress) {
#ifdef DEBUG
    Serial.println("[Kenwood Encoder] Initialized");
#endif
}

void KenwoodEncoder::setKenwoodDeviceAddress(uint8_t address) {
    _deviceAddress = address;
}

NecMessage KenwoodEncoder::buildNecMessage(SwcButton button) {
#ifdef DEBUG_KENWOOD
    String textOut = "Button Code Recieved: " + button;
    Serial.println(textOut);
#endif
    return buildNecMessage(KenwoodConstants::kDefaultCodes[button]);
}

NecMessage KenwoodEncoder::buildNecMessage(uint8_t kenwoodCode) {
    uint8_t messageBytes[4];

    messageBytes[0] = _deviceAddress;
    messageBytes[1] = ~_deviceAddress;
    messageBytes[2] = kenwoodCode;
    messageBytes[3] = ~kenwoodCode;

#ifdef DEBUG_KENWOOD_BYTES
    String textOut = "Array Contents: " 
        + String(messageBytes[0]) + " " 
        + String(messageBytes[1]) + " "
        + String(messageBytes[2]) + " "
        + String(messageBytes[3]);
    Serial.println(textOut);
#endif

    return NecMessage(messageBytes);
}
