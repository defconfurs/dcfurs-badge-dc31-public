#ifndef __Arduino_is31fl3737
#define __Arduino_is31fl3737

#include "Arduino.h"
#include "Wire.h"
#include "is31fl3737.h"
#include "is31fl3737_abm.h"

class Arduino_is31fl3737
{
  public:
    IS31FL3737_ABM ABM1;
    IS31FL3737_ABM ABM2;
    IS31FL3737_ABM ABM3;
    // Constructor
    Arduino_is31fl3737();  // Initialize using IS31FL3737_I2C_BASE_ADDR
    Arduino_is31fl3737(uint8_t address); // Initialize using specified addres
    Arduino_is31fl3737(uint8_t ADDR2, uint8_t ADDR1); // Initialize using ADDR_[GND|SCL|SDA|VCC] macros
    // Initialize object and I2C
    void Init(void);
    // Set global current control register.
    void SetGCC(uint8_t gcc);
    // Set LED state: ON/OFF. Could be set ALL / CS / SW.
    void SetLEDState (uint8_t cs, uint8_t sw, IS31FL3737_LED_STATE state);
    // Set LED PWM duty value. Could be set ALL / CS / SW.
    void SetLEDPWM (uint8_t cs, uint8_t sw, uint8_t value);
    // Set LED operating mode: PWM/ABM1,2,3. Could be set ALL / CS / SW.
    void SetLEDMode (uint8_t cs, uint8_t sw, IS31FL3737_LED_MODE mode);
    // Write ABM structure parameters to device registers.
    void WriteConfigABM(uint8_t n);
    // Start ABM mode operation.
    void StartABM();
    // Set SW Pull-Up register.
    void SetSWPUR (IS31FL3737_RESISTOR resistor);
    // Set CS Pull-Down register.
    void SetCSPDR (IS31FL3737_RESISTOR resistor); 
    // Get status of LED.
    IS31FL3737_LED_STATUS GetLEDStatus (uint8_t cs, uint8_t sw, unsigned long delayTime = 1000);
    // Set LED state for all LED's from buffer.
    void SetState (uint8_t *states);
    // SET LED PWM duty value for all LED's from buffer.
    void SetPWM (uint8_t *values);
  protected:
    IS31FL3737 device;
    static uint8_t i2c_write_reg(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *buffer, uint8_t count);
    static uint8_t i2c_read_reg(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *buffer, uint8_t count);
};

#endif //__Arduino_is31fl3737
