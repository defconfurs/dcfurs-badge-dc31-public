#include "Arduino_is31fl3737.h"

Arduino_is31fl3737 disp(0xA0);

void setup() 
{
    Serial.begin(9600);  // start Serial for output
    Serial.println("Starting");
    disp.Init();
    disp.SetGCC(0xF0);
    disp.SetLEDPWM(IS31FL3737_CS, IS31FL3737_SW, 255);
    disp.SetLEDState(IS31FL3737_CS, IS31FL3737_SW, IS31FL3737_LED_STATE_ON);
}

void loop() 
{
    //  //waiting for input
    for (uint8_t i_sw = 11; i_sw<12; i_sw--){
        for (uint8_t i_cs = 0; i_cs<12; i_cs++){
            Serial.print(disp.GetLEDStatus(i_cs, i_sw));
        }
        Serial.println("");
    }
    Serial.println("################");
    delay(3000);
}
