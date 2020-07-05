#include "LocalDefine.h"
#include "SwcButtonEnum.h"
#include "NecMessage.h"
#include "SwcInterface.h"
#include "KenwoodEncoder.h"
#include "NecInterface.h"

SwcInterface swcInterface;
KenwoodEncoder kenwoodEncoder;
NecInterface necInterface;

SwcButton lastButton = SwcButton::EndOfEnum;

void setup()
{
#ifdef DEBUG
    Serial.begin(9600);
    while (!Serial) {
    }
#endif
    swcInterface = SwcInterface();
    kenwoodEncoder = KenwoodEncoder();
    necInterface = NecInterface();

    necInterface.sendNecMessage(kenwoodEncoder.buildNecMessage(SwcButton::VolumeUp));
}

void loop()
{
    SwcButton buttonIndex = swcInterface.readSwc();

#ifdef DEBUG_SWC_BUTTONS
    swcInterface.printSwc(buttonIndex);
#endif

    if (buttonIndex == SwcButton::Error) {
    }
    else if (buttonIndex != lastButton) {
        if ((buttonIndex < SwcButton::EndOfEnum) && necInterface.readyForNewMessage()) {
            necInterface.sendNecMessage(kenwoodEncoder.buildNecMessage(buttonIndex));
        }
        lastButton = buttonIndex;
    }
    else if ((buttonIndex < SwcButton::EndOfEnum) && necInterface.readyForNewMessage()) {
        necInterface.sendNecRepeat();
    }

    necInterface.tick();
}
