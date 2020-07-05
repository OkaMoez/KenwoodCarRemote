#include <Arduino.h>
#include "LocalDefine.h"
#include "SwcInterface.h"

SwcInterface::SwcInterface() :
    SwcInterface(SwcConstants::kDefaultSwcInputPin) {
}

SwcInterface::SwcInterface(uint8_t inputPin) : 
    _inputPin(inputPin) {
#ifdef DEBUG
    Serial.println("[Swc Interface] Initialized");
#endif
}

SwcButton SwcInterface::readSwc() {
    int rawSwc = analogRead(_inputPin);

    for (uint8_t i = 0; i < 2; i++)  {
        if (rawSwc != analogRead(_inputPin)) {
            return;
        }
        delay(SwcConstants::kDefaultSwcVerificationDelay);
    }

#ifdef DEBUG_SWC_RAW
    String textOut = "Raw Pin: " + String(rawSwc);
    Serial.println(textOut);
#endif
    if (rawSwc <= 0) return SwcButton::Error;
    if (rawSwc < 20) return SwcButton::VolumeDown; 
    if (rawSwc < 45) return SwcButton::VolumeUp; 
    if (rawSwc < 85) return SwcButton::SeekUp; 
    if (rawSwc < 150) return SwcButton::SeekDown; 
    if (rawSwc < 250) return SwcButton::Mode; 
    if (rawSwc < 400) return SwcButton::Mute;
    if (rawSwc < 700) return SwcButton::Open;
    return SwcButton::Error;
}

void SwcInterface::printSwc(SwcButton buttonPressed) {
    String buttonName = "Button name: ";
    switch(buttonPressed){
        case SwcButton::Open:
#ifdef DEBUG_SWC_BUTTONS_VERBOSE
            buttonName += "No Button";
            break;
#else
            return;
#endif 
        case SwcButton::VolumeUp:
            buttonName += "Volume Up";
            break;
        case SwcButton::VolumeDown:
            buttonName += "Volume Down";
            break; 
        case SwcButton::SeekUp:
            buttonName += "Seek Up";
            break;
        case SwcButton::SeekDown:
            buttonName += "Seek Down";
            break;
        case SwcButton::Mode:
            buttonName += "Mode";
            break;
        case SwcButton::Mute:
            buttonName += "Mute";
            break;
        default:
#ifdef DEBUG_SWC_BUTTONS_VERBOSE
            buttonName += "Error";
            break;
#else
            return;
#endif
    }

    Serial.println(buttonName);
}
