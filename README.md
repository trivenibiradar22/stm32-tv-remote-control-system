# STM32 TV Remote Control System

## Overview
This project simulates a Television Remote Control System using STM32 MCU. The system allows users to perform TV operations such as Power ON/OFF, Channel Up/Down, Volume Up/Down, and Mute using push-buttons.

The LCD display shows TV status, volume level, and mute status, while the 7-segment display shows the current channel number. A relay simulates TV power control and a buzzer provides audio feedback for button operations.

---

## Features
- TV Power ON/OFF Control
- Channel Up/Down Control
- Volume Up/Down Control
- Mute Function
- LCD Status Display
- 7-Segment Channel Display
- Relay-Based TV Power Simulation
- Buzzer Alert Feedback

---

## Components Used
- STM32F410RBTx MCU
- Push Buttons
- 16x2 LCD Display
- 2-Digit 7-Segment Display
- Relay Module
- BC547 Transistor
- Buzzer
- 1N4007 Diode
- Resistors
- Connecting Wires

---

## Software Used
- STM32CubeIDE
- STM32CubeMX
- Proteus

---

## Pin Configuration

### Push Buttons

| Function | GPIO Pin |
|---|---|
| Power ON/OFF | PA0 |
| Volume Up | PA1 |
| Volume Down | PA4 |
| Channel Up | PB12 |
| Channel Down | PB13 |
| Mute | PB14 |

---

### Output Devices

| Device | GPIO Pin |
|---|---|
| Buzzer | PB5 |
| Relay | PA5 |

---

### LCD Connections

| LCD Signal | GPIO Pin |
|---|---|
| RS | PB6 |
| EN | PB8 |
| D4 | PB9 |
| D5 | PB10 |
| D6 | PB1 |
| D7 | PB0 |

---

## Working Principle
1. User presses push-buttons for TV operations.
2. STM32 reads button inputs through GPIO pins.
3. LCD displays TV status and information.
4. 7-segment display shows current channel number.
5. Relay simulates TV power control.
6. Buzzer provides audio feedback.

---

## Project Structure

STM32_Code/
Proteus_Simulation/
Project_Report/
Screenshots/
Demo_Video/
README.md

---

## Future Improvements
- IR Remote Integration
- Wireless Remote Control
- IoT-Based TV Control
- Mobile App Integration

---

## Author
Triveni Biradar

---

## License
This project is developed for educational and learning purposes.
