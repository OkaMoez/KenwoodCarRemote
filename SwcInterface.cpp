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
#ifdef DEBUG_SWC_RAW
    String textOut = "Raw Pin: " + String(rawSwc);
    Serial.println(textOut);
#endif
    if (rawSwc <= 0) return SwcError;
    if (rawSwc < 10) return SwcVolumeDown; 
    if (rawSwc < 20) return SwcVolumeUp; 
    if (rawSwc < 40) return SwcSeekUp; 
    if (rawSwc < 70) return SwcSeekDown; 
    if (rawSwc < 120) return SwcMode; 
    if (rawSwc < 250) return SwcMute;
    if (rawSwc < 400) return SwcOpen;
    return SwcError;
}

void SwcInterface::printSwc(SwcButton buttonPressed) {
    String buttonName = "Button name: ";
    switch(buttonPressed){
        case SwcOpen:
#ifdef DEBUG_SWC_BUTTONS_VERBOSE
            buttonName += "No Button";
            break;
#else
            return;
#endif 
        case SwcVolumeUp:
            buttonName += "Volume Up";
            break;
        case SwcVolumeDown:
            buttonName += "Volume Down";
            break; 
        case SwcSeekUp:
            buttonName += "Seek Up";
            break;
        case SwcSeekDown:
            buttonName += "Seek Down";
            break;
        case SwcMode:
            buttonName += "Mode";
            break;
        case SwcMute:
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
