#define VERBOSE_SWC

#include <stdint.h>
#include "SwcButtonEnum.h"
#include "SwcInterface.h"
#include "KenwoodNecInterface.h"

SwcInterface swcInterface;
KenwoodNecInterface kenwoodInterface;

void setup()
{
    Serial.begin(9600);
    swcInterface = SwcInterface();
    kenwoodInterface = KenwoodNecInterface();

#ifdef DEBUG_SWC or DEBUG_NEC
    while (!Serial) {
    }
#endif
}

void loop()
{
    SwcButton buttonIndex = swcInterface.readSwc();
    swcInterface.printSwc(buttonIndex);
    
    if ((0 <= buttonIndex) && (buttonIndex < SwcEndOfEnum)) {
        kenwoodInterface.sendSwcToKenwood(buttonIndex);
    }
    else {
        kenwoodInterface.clearLastSwcButtonToKenwood();
    }
}
