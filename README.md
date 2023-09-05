# Vehicle Control System

## Table of Contents

1. [ECU 1](#ECU-1)
   - [LCD Interface](#lcd-interface)
   - [Potentiometer Interface](#potentiometer-interface)
   - [Keypad Interface](#keypad-interface)
   - [Speed Limit Control](#speed-limit-control)
2. [ECU 2](#ECU-2)
   - [EEPROM (I2C) Interface](#eeprom-i2c-interface)
   - [SPI Communication](#spi-communication)
   - [Speed Limit Management](#speed-limit-management)
   - [Gear Control](#gear-control)
   - [Speed Control](#speed-control)
   - [Safety Measures](#safety-measures)
3. [Layered Architecture](#Layered-Architecture)
4. [Team Members](#Team-Members)



## 1. ECU 1 <a name="ECU-1"></a>

Electronic Control Unit 1, or ECU 1, plays a vital role in controlling various aspects of the vehicle's operation. Here are its key interfaces and functionalities:

### LCD Interface <a name="lcd-interface"></a>

- **Main Display:** The LCD continuously updates and displays the current speed and gear (P, R, N, D).
- **Speed Limit Display:** Indicates whether the speed limit is turned on (display "ON") or off (display "OFF").

### Potentiometer Interface <a name="potentiometer-interface"></a>

- **Speed Control:** ECU 1 reads the potentiometer's analog input to determine the desired speed in "D" mode.

### Keypad Interface <a name="keypad-interface"></a>

- **Gear Control:** The keypad is scanned for user input. When a gear is selected, ECU 1 updates the gear display on the LCD.
- **Speed Limit Control:** ECU 1 provides a menu option for the driver to enable or disable the speed limit feature in "D" mode.

### Speed Limit Control <a name="speed-limit-control"></a>

- When the speed limit is enabled, ECU 1 initiates communication with ECU 2 to retrieve the speed limit value from EEPROM.

## 2. ECU 2 <a name="ECU-2"></a>

Electronic Control Unit 2, or ECU 2, complements the functionality of ECU 1 and manages specific tasks related to speed limit control and communication. Here's an overview of its interfaces and functions:

### EEPROM (I2C) Interface <a name="eeprom-i2c-interface"></a>

- **Speed Limit Management:** ECU 2 uses the I2C protocol to interact with the EEPROM, which stores speed limit information.
  - It can read and write data to/from the EEPROM.

### SPI Communication <a name="spi-communication"></a>

- **Speed Limit Management:** The communication between ECU 1 and ECU 2 is established using SPI.
  - ECU 1 serves as the master, and ECU 2 acts as the slave.
  - When the driver selects "Enable Speed Limit" on ECU 1, it sends a request to ECU 2 via SPI.
  - ECU 2 reads the speed limit value from the EEPROM.
  - ECU 2 then sends the speed limit value to ECU 1 via SPI, confirming the activation of the speed limit feature.
  - ECU 1 enforces the speed limit in "D" mode based on the received value. It compares the current speed with the limit and adjusts vehicle performance accordingly to stay within the limit.

### Gear Control <a name="gear-control"></a>

- The keypad on ECU 1 allows the driver to select different gears (P, R, N, D).
- ECU 1 updates the LCD display to reflect the chosen gear, ensuring clear communication to the driver.

### Speed Control <a name="speed-control"></a>

- ECU 1 monitors the potentiometer for speed adjustment.

### Safety Measures <a name="safety-measures"></a>

- In "R" (Reverse) mode, ECU 1 enforces a maximum speed of 30 KM/H, regardless of the speed limit setting, to ensure safety during backward movement.

## 3. Layered Architecture <a name="Layered-Architecture"></a>
![image](https://github.com/mohameddmahmoudd/Vehicle-Control-System/assets/52659572/5ee5af2a-0fe5-4222-ae07-aca344d18887)

## 4.Team Members <a name="Team-Members"></a>
![image](https://github.com/mohameddmahmoudd/Vehicle-Control-System/assets/52659572/0425376f-92c2-487d-b221-fd2c9848f0c1)

