#include "Arduino_is31fl3737.h"

Arduino_is31fl3737 disp(ADDR_GND, ADDR_GND);

uint8_t sequenceToCoordinatesCS(uint8_t number){
    if (number >= 144){
        number -= 144;
    }
    return (number%12);
}
uint8_t sequenceToCoordinatesSW(uint8_t number){
    if (number >= 144){
        number -= 144;
    }
    return (number/12);
}

void setup() 
{
    Serial.begin(9600);  // start Serial for output
    Serial.println("Starating");
    Serial.print("Address resolved as ");
    Serial.println(IS31FL3737_I2C_ADDR(ADDR_GND, ADDR_GND));
    disp.Init();
    disp.SetGCC(0xF0);
    for(uint8_t i_cs = 0; i_cs<IS31FL3737_CS; i_cs++){
        for(uint8_t i_sw = 0; i_sw<IS31FL3737_SW; i_sw++){
            disp.SetLEDPWM(i_cs, i_sw, 0);
            disp.SetLEDState(i_cs, i_sw, IS31FL3737_LED_STATE_ON);
        }
    }
}

void loop() 
{
     //waiting for input
    for(uint8_t i = 0; i<(12*12); i++){
        disp.SetLEDPWM(sequenceToCoordinatesCS(i), sequenceToCoordinatesSW(i), 0);
        disp.SetLEDPWM(sequenceToCoordinatesCS(i+1), sequenceToCoordinatesSW(i+1), 51);
        disp.SetLEDPWM(sequenceToCoordinatesCS(i+2), sequenceToCoordinatesSW(i+2), 102);
        disp.SetLEDPWM(sequenceToCoordinatesCS(i+3), sequenceToCoordinatesSW(i+3), 153);
        disp.SetLEDPWM(sequenceToCoordinatesCS(i+4), sequenceToCoordinatesSW(i+4), 204);
        disp.SetLEDPWM(sequenceToCoordinatesCS(i+5), sequenceToCoordinatesSW(i+5), 255);

        delay(20);
    }
}
