#include <Arduino.h>
#include "SwcInterface.h"

SwcInterface::SwcInterface() :
    SwcInterface(SwcConstants::kDefaultSwcInputPin) {
}

SwcInterface::SwcInterface(uint8_t inputPin) : 
    _inputPin(inputPin) {
}

SwcButton SwcInterface::readSwc() {
    int rawSwc = analogRead(_inputPin);
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
    while (!Serial) {

    }

    String buttonName;
    switch(buttonPressed){
        case SwcOpen:
            buttonName = "No Button\r";
            break;
        case SwcVolumeUp:
            buttonName = "Volume Up\n";
            break;
        case SwcVolumeDown:
            buttonName = "Volume Down\n";
            break; 
        case SwcSeekUp:
            buttonName = "Seek Up\n";
            break;
        case SwcSeekDown:
            buttonName = "Seek Down\n";
            break;
        case SwcMode:
            buttonName = "Mode\n";
            break;
        case SwcMute:
            buttonName = "Mute\n";
            break;
        default:
            buttonName = "Error\r";
            break;
    }

    Serial.print("button name: ");
    Serial.print(buttonName);
}
