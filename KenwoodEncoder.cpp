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
    String textOut = "Button Code Recieved: " + String(static_cast<uint8_t>(button));
    Serial.println(textOut);
#endif

    NecMessage necMessage = NecMessage();

    if (button < SwcButton::EndOfEnum) {
        necMessage = buildNecMessage(KenwoodConstants::kDefaultCodes[static_cast<uint8_t>(button)]);
    }

    return necMessage;
}

NecMessage KenwoodEncoder::buildNecMessage(uint8_t kenwoodCode) {
    uint8_t messageBytes[4];

    messageBytes[0] = ~kenwoodCode;
    messageBytes[1] = kenwoodCode;
    messageBytes[2] = ~_deviceAddress;
    messageBytes[3] = _deviceAddress;

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
