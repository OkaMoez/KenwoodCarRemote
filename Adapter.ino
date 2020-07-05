#include "LocalDefine.h"
#include "SwcButtonEnum.h"
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
}

void loop()
{
    SwcButton buttonIndex = swcInterface.readSwc();

#ifdef DEBUG_SWC_BUTTONS
    swcInterface.printSwc(buttonIndex);
#endif

    if (buttonIndex != lastButton) {
        necInterface.sendNec(kenwoodEncoder.buildNecMessage(buttonIndex));
    }
    else {
        necInterface.sendNecRepeat();
    }

    buttonIndex = lastButton;
}
