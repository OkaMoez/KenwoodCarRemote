#include "SwcButtonEnum.h"
#include "SwcInterface.h"
#include "KenwoodEncoder.h"
#include "NecInterface.h"

SwcInterface swcInterface;
KenwoodEncoder kenwoodEncoder;
NecInterface necInterface;

void setup()
{
    Serial.begin(9600);
    swcInterface = SwcInterface();
    kenwoodEncoder = KenwoodEncoder();
    necInterface = NecInterface();
}

void loop()
{
    //SwcButton buttonIndex = swcInterface.readSwc();
    //swcInterface.printSwc(buttonIndex);
    
}
