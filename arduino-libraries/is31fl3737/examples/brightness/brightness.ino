#include "Arduino_is31fl3737.h"

Arduino_is31fl3737 disp();

void setup() 
{
    Serial.begin(9600);  // start Serial for output
    Serial.println("Starating");
    disp.Init();
    disp.SetGCC(0xF0);
    for(uint8_t i_cs = 0; i_cs<12; i_cs++){
        for(uint8_t i_sw = 0; i_sw<12; i_sw++){
            disp.SetLEDPWM(i_cs, i_sw, 255);
            disp.SetLEDState(i_cs, i_sw, IS31FL3737_LED_STATE_ON);
        }
    }
}

void loop() 
{
     //waiting for input
    while (Serial.available() == 0);
    uint8_t val = (uint8_t)Serial.parseInt();
    Serial.parseInt(); // Parse newline
    for(uint8_t i_cs = 0; i_cs<12; i_cs++){
        for(uint8_t i_sw = 0; i_sw<12; i_sw++){
            disp.SetLEDPWM(i_cs, i_sw, val);
        }
    }
    Serial.print("Setting PWM to:");
    Serial.println(val);
}
