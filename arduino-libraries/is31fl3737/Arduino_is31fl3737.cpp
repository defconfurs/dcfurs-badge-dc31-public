#include "Arduino_is31fl3737.h"

#include "Arduino.h"
#include "Wire.h"
#include "is31fl3737.h"
#include "is31fl3737_abm.h"

/******************************Private*****************************/
uint8_t Arduino_is31fl3737::i2c_write_reg(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *buffer, uint8_t count) {
	Wire.beginTransmission(i2c_addr);
	Wire.write((byte) reg_addr);
	for (uint8_t i = 0; i < count; i++) {
		Wire.write((byte)(buffer[i]));
	}
	Wire.endTransmission();
	return 1;
}

uint8_t Arduino_is31fl3737::i2c_read_reg(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *buffer, uint8_t count) {
	Wire.beginTransmission(i2c_addr);
	Wire.write((byte) reg_addr);
	Wire.endTransmission();

	Wire.requestFrom((byte) i2c_addr, (size_t) count);
	uint8_t i = 0;
	while (Wire.available())    // slave may send less than requested
	{
		buffer[i++] = (uint8_t) Wire.read(); // receive a byte as character
	}
	return 1;
}

/******************************Public*****************************/
Arduino_is31fl3737::Arduino_is31fl3737() {
	device.address = IS31FL3737_I2C_ADDR(ADDR_GND, ADDR_GND)>>1;
	device.i2c_write_reg = &(Arduino_is31fl3737::i2c_write_reg);
	device.i2c_read_reg = &(Arduino_is31fl3737::i2c_read_reg);
}
Arduino_is31fl3737::Arduino_is31fl3737(uint8_t address) {
	device.address = address>>1;
	device.i2c_write_reg = &(Arduino_is31fl3737::i2c_write_reg);
	device.i2c_read_reg = &(Arduino_is31fl3737::i2c_read_reg);
}
Arduino_is31fl3737::Arduino_is31fl3737(uint8_t ADDR2, uint8_t ADDR1) {
	device.address = (uint8_t)IS31FL3737_I2C_ADDR(ADDR2, ADDR1)>>1;
	device.i2c_write_reg = &(Arduino_is31fl3737::i2c_write_reg);
	device.i2c_read_reg = &(Arduino_is31fl3737::i2c_read_reg);
}
void Arduino_is31fl3737::Init(void) {
//	Wire.begin(device.address);
	Wire.begin();
	Wire.setClock(400000);
	IS31FL3737_Init(&device);
}
void Arduino_is31fl3737::SetGCC(uint8_t gcc) {
	IS31FL3737_SetGCC(&device, gcc);
}
void Arduino_is31fl3737::SetLEDState(uint8_t cs, uint8_t sw, IS31FL3737_LED_STATE state) {
	IS31FL3737_SetLEDState(&device, cs, sw, state);
}
void Arduino_is31fl3737::SetLEDPWM(uint8_t cs, uint8_t sw, uint8_t value) {
	IS31FL3737_SetLEDPWM(&device, cs, sw, value);
}
void Arduino_is31fl3737::SetLEDMode(uint8_t cs, uint8_t sw, IS31FL3737_LED_MODE mode){
	IS31FL3737_SetLEDMode (&device, cs, sw, mode);
}
void Arduino_is31fl3737::WriteConfigABM(uint8_t n){
	IS31FL3737_ABM *config;
	IS31FL3737_ABM_NUM ABMnum;
	switch (n){
		case 1:
			config = &ABM1;
			ABMnum = IS31FL3737_ABM_NUM_1;
			break;
		case 2:
			config = &ABM2;
			ABMnum = IS31FL3737_ABM_NUM_2;
			break;
		case 3:
			config = &ABM3;
			ABMnum = IS31FL3737_ABM_NUM_3;
			break;
		default:
			return;
	}
    IS31FL3737_ConfigABM(&device, ABMnum, config);
}
void Arduino_is31fl3737::SetSWPUR (IS31FL3737_RESISTOR resistor){
	IS31FL3737_SetSWPUR(&device, resistor);
}
void Arduino_is31fl3737::SetCSPDR (IS31FL3737_RESISTOR resistor){
	IS31FL3737_SetCSPDR(&device, resistor); 
}
IS31FL3737_LED_STATUS Arduino_is31fl3737::GetLEDStatus (uint8_t cs, uint8_t sw, unsigned long delayTime/*=1000*/){
	//refresh Open Short Detection info
  	IS31FL3737_WritePagedReg (&device, IS31FL3737_CR, IS31FL3737_CR_SSD);
  	IS31FL3737_WritePagedReg (&device, IS31FL3737_CR, IS31FL3737_CR_OSD|IS31FL3737_CR_SSD);
	// wait a bit, because detection needs it. Can be changed according to pullup and pulldown resistors.
	delayMicroseconds(delayTime);
	// Read the info out
    return(IS31FL3737_GetLEDStatus (&device, cs, sw));
}
// void Arduino_is31fl3737::SetState (uint8_t *states){
//     void IS31FL3737_SetState (IS31FL3737 *device, uint8_t *states);
// }
// void Arduino_is31fl3737::SetPWM (uint8_t *values){
//     void IS31FL3737_SetPWM (IS31FL3737 *device, uint8_t *values);
// }
void Arduino_is31fl3737::StartABM(){
    IS31FL3737_StartABM (&device);
}
