#include "Arduino_is31fl3737.h"

Arduino_is31fl3737 disp(0xA0);

void setup() 
{
    Serial.begin(9600);  // start Serial for output
    Serial.println("Starating");
    disp.Init();
    disp.SetGCC(0xF0);
    for(uint8_t i_cs = 0; i_cs<12; i_cs++){
        for(uint8_t i_sw = 0; i_sw<12; i_sw++){
            disp.SetLEDState(i_cs, i_sw, IS31FL3737_LED_STATE_ON);
        }
    }
    // Configure all matrix LEDs to work in disp.ABM mode.
    disp.SetLEDMode(IS31FL3737_CS, IS31FL3737_SW, IS31FL3737_LED_MODE_ABM1);
    // Set ABM mode structure parameters.
    disp.ABM1.T1 = IS31FL3737_ABM_T1_840MS;
    disp.ABM1.T2 = IS31FL3737_ABM_T2_420MS;
    disp.ABM1.T3 = IS31FL3737_ABM_T3_1680MS;
    disp.ABM1.T4 = IS31FL3737_ABM_T4_420MS;
    disp.ABM1.Tbegin = IS31FL3737_ABM_LOOP_BEGIN_T4;
    disp.ABM1.Tend = IS31FL3737_ABM_LOOP_END_T3;
    disp.ABM1.Times = IS31FL3737_ABM_LOOP_FOREVER;

    // Write ABM structure parameters to device registers.
    disp.WriteConfigABM(1);
    // Start ABM mode operation.
    disp.StartABM();
}

void loop() 
{
}
