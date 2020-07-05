#include <Arduino.h>
#include "KenwoodEncoder.h"
#include "NecMessage.h"

KenwoodEncoder::KenwoodEncoder() : 
    KenwoodEncoder(
        KenwoodConstants::kDefaultDeviceAddress) {
}

KenwoodEncoder::KenwoodEncoder(uint8_t deviceAddress) {
    _deviceAddress = deviceAddress;
}

void KenwoodEncoder::setKenwoodDeviceAddress(uint8_t address) {
    _deviceAddress = address;
}

NecMessage KenwoodEncoder::buildNecMessage(SwcButton button) {
#ifdef DEBUG_NEC
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

#ifdef VERBOSE_BITS
    String textOut = "Array Contents: " 
        + messageBytes[0] + " " 
        + messageBytes[1] + " "
        + messageBytes[2] + " "
        + messageBytes[3];
    Serial.println(textOut);
#endif

    return NecMessage(messageBytes);
}
