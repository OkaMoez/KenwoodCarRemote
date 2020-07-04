#include "SwcInterface.h"

SwcInterface::SwcInterface() :
    SwcInterface(SwcConstants::kDefaultSwcInputPin) {
}

SwcInterface::SwcInterface(u8 inputPin) : 
    _inputPin(inputPin) {
}

SwcButton SwcInterface::readSwc() {
    int rawSwc = analogRead(SWC_PIN);
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

#ifdef DEBUG_SWC
void SwcInterface::printSwc(SwcButton buttonPressed) {
#ifndef VERBOSE_SWC
    if (buttonPressed == _lastSwcButtonPressed) {
        return;
    }
#endif

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
    
    _lastSwcButtonPressed = buttonPressed;
}
#endif
