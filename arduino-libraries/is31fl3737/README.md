# Arduino IS31FL3737 library v1.0.1
This is arduino library for IC **ISSI IS31FL3737** based on [IS31FL3737 C library](https://github.com/kkostyan/is31fl3737) by [kkostyan](https://github.com/kkostyan) (__Big thanks to you!__).

Feel free to use this library, open issues, find bugs and create patches.

To fully understand implemented features, it is strongly adviced to carefully read ISSI IS31FL3737A [datasheet](https://www.zlgmcu.com/data/upload/file/pdrive/IS31FL3737A.pdf).

This library is licensed under **GNU GPL v.3**.

## Examples
This library comes with prepared examples:
- **Blink** - Blinks all LEDs on and off with 1s period.
- **BlinkABM** - Blinking all LEDs using **A**uto **B**reath **M**ode.
- **Brightness** - Demonstrating PWM mode. Listening serial and setting received PWM to all LEDs.
- **OSD** - Demonstrating **O**pen/**S**hort **D**etection. Periodically printing state of individual LEDs in matrix.
- **Runner** - Basic animation using PWM mode.

## Function documentation
``` c++
Arduino_is31fl3737();  // Initialize using IS31FL3737_I2C_BASE_ADDR
Arduino_is31fl3737(uint8_t address); // Initialize using specified addres
Arduino_is31fl3737(uint8_t ADDR2, uint8_t ADDR1); // Initialize using ADDR_[GND|SCL|SDA|VCC] macros

void Init(void); // Initialize object and arduino Wire with 400000 clock
void SetGCC(uint8_t gcc); // Set global current control register.
void SetLEDState (uint8_t cs, uint8_t sw, IS31FL3737_LED_STATE state); // Set LED state: ON/OFF. Could be set ALL / CS / SW.
void SetLEDPWM (uint8_t cs, uint8_t sw, uint8_t value); // Set LED PWM duty value. Could be set ALL / CS / SW.
void SetLEDMode (uint8_t cs, uint8_t sw, IS31FL3737_LED_MODE mode); // Set LED operating mode: PWM/ABM1,2,3. Could be set ALL / CS / SW.
void WriteConfigABM(uint8_t n); // Write ABM structure parameters to device registers.
void StartABM(); // Start ABM mode operation.
void SetSWPUR (IS31FL3737_RESISTOR resistor); // Set SW Pull-Up register.
void SetCSPDR (IS31FL3737_RESISTOR resistor); // Set CS Pull-Down register.
IS31FL3737_LED_STATUS GetLEDStatus (uint8_t cs, uint8_t sw, unsigned long delayTime = 1000); // Get status of LED.
void SetState (uint8_t *states); // Set LED state for all LED's from buffer.
void SetPWM (uint8_t *values); // SET LED PWM duty value for all LED's from buffer.

IS31FL3737_ABM ABM1; // Object storing ABM1 settings
IS31FL3737_ABM ABM2; // Object storing ABM2 settings
IS31FL3737_ABM ABM3; // Object storing ABM3 settings
```
**NOTE**: Functions `SetLEDState`, `SetLEDPWM` and `SetLEDMode` have `cs` and `sw` arguments specifying address of LED. If passed number is larger than number of cs/sw, all cs/sw are selected. For example:
```
SetLEDPWM(16, 2, 255);
```
This code switches all LEDs with sw==2.

---
#### TODO:
- Find out a way to test `SetSWPUR` and `SetCSPDR` feature
- Implement and test `SetState` feature
- Implement and test `SetPWM` feature
