# MCU LAB5: UART and ADC

## Introduction

I am a student of the Microcontroller course at HCMUT. In this lab, I worked with STM32CubeIDE and Proteus simulation, using the STM32F103C6 microcontroller.
Lab 5 focuses on understanding and configuring **UART communication** and **ADC peripheral** on STM32. The project involves designing a simple state machine to handle UART communication and using ADC to read voltage values. Key challenges included setting up UART interrupts for receiving data, parsing commands from the UART terminal, and integrating ADC readings into the communication protocol. Through this lab, I learned how to configure and use UART for serial communication, process received data using a finite state machine (FSM), and utilize ADC to measure analog signals.


---

## Demonstration and Report

-   **[Proteus Demo Video (Google Drive)](https://drive.google.com/file/d/1462wia7un_481wpNqJGVyD1oNw9T5Gq0/view?usp=sharing)**

-   **[PDF Report (Google Drive)](https://drive.google.com/file/d/1fsb4f9F8hE8ivQaXZ-J2M6pou492CT2M/view?usp=sharing)**

---

## Proteus Schematic Image
<img width="1400" height="725" alt="image" src="https://github.com/user-attachments/assets/b41e8795-4635-4e68-b687-6913202841a9" />

This Proteus schematic represents a simple embedded system using the STM32F103C6 microcontroller. The design integrates UART communication, ADC voltage measurement, and a debug LED for system status indication.

### Key Components:
1. **STM32F103C6 Microcontroller (U1)**:
   - Central processing unit for the system.
   - Configured for UART communication and ADC voltage measurement.
   - Pins:
     - `ADC0`: Connected to the ADC input for voltage measurement.
     - `UART_TX` and `UART_RX`: Used for serial communication.
     - `DEBUG_LED`: Indicates system status.

2. **UART Communication**:
   - Virtual terminal connected to `UART_TX` and `UART_RX`.
   - Used for sending and receiving commands and data.

3. **ADC Voltage Measurement**:
   - Voltage divider circuit with potentiometer (RV1) and resistor (R1).
   - Operational amplifier (U2) provides stable input to the ADC pin (`ADC0`).(a voltage follower circuit)
   - Allows measurement of adjustable voltage values.

4. **Debug LED (D1)**:
   - Connected to GPIO pin `DEBUG_LED`.
   - Used for visual indication of system activity or errors.

### Functionality:
- The microcontroller reads analog voltage values from the ADC pin (`ADC0`) and processes them.
- UART communication enables interaction with the system via commands sent from the virtual terminal.
- The debug LED provides feedback on the system's operational state.

---

## .ioc File Image (STM32CubeMX)
<img width="673" height="631" alt="image" src="https://github.com/user-attachments/assets/cd76ad24-fb73-4262-9e97-e3d36e8cba15" />

---

## Command parser FSM and communication FSM
<img width="883" height="384" alt="image" src="https://github.com/user-attachments/assets/18d059d7-33b6-4a18-ae24-124866984bb9" />

### 1. Command Parser FSM
- **States**:
  - `CMD_WAITING`: Waits for the start of a command (`!`).
  - `CMD_GET`: Collects characters until the end of the command (`#`).
- **Transitions**:
  - `in_byte == '!'`: Switch to `CMD_GET` and reset the buffer.
  - `in_byte == '#'`: Process the command and return to `CMD_WAITING`.

### 2. Communication FSM
- **States**:
  - `INIT`: Initializes the system and transitions to `WAITING`.
  - `WAITING`: Waits for a valid command (`RST` or `OK`).
  - `SENDING`: Sends data (e.g., ADC value) to the virtual terminal.
  - `WAIT_ACK`: Waits for acknowledgment (`OK`) or resends data after 3 seconds.
  - `UPDATING`: Continuously update and send the newest ADC value to the virtual terminal.

These FSMs ensure robust handling of UART commands and seamless integration of ADC data into the communication protocol.

---

## Implementation Details
### 1. UART Configuration
- **Baud Rate**: 9600 bps.
- **Mode**: Asynchronous, 8 data bits, no parity, 1 stop bit.
- **Interrupts**: Enabled for receiving data using `HAL_UART_Receive_IT`.
- **Buffer**: Circular buffer implemented to store incoming data efficiently.
- **Special Character Handling**:
  - **Backspace (`\b`)**: Removes the last character from the buffer and updates the terminal display.
  - **Invalid Characters**: Ignored, ensuring only valid commands are processed.

### 2. ADC Configuration
- **Channel**: ADC Channel 0 connected to the voltage measurement circuit.
- **Resolution**: 12-bit, providing values from 0 to 4095.
- **Mode**: Continuous conversion for real-time voltage monitoring.
- **Voltage Reference**: 5V, allowing conversion to actual voltage values.
### 3. Supported Commands
- **`!RST#`**: Resets the system and sends the current ADC value over UART.
- **`!OK#`**: Acknowledges the last command and transitions back to `WAITING`.
- **`!UPD#`**: Enables periodic ADC value updates over UART.

---

## Source Folder Structure

The main code for Lab 5 is organized in the `Core/` directory as follows:
```text
Core/
│
├── Src/                        # Main embedded source code
│   ├── uart.c                  # UART communication logic
│   ├── software_timer.c        # Common software timer module
│   ├── main.c                  # Entry point, main application logic
│   ├── stm32f1xx_hal_msp.c     # HAL MSP initialization
│   ├── stm32f1xx_it.c          # Interrupt handlers
│   ├── syscalls.c              # System call stubs
│   ├── sysmem.c                # System memory allocation
│   └── system_stm32f1xx.c      # System clock and configuration
│
├── Inc/                        # Header files for each module and main
│   ├── uart.h                  # UART communication definitions
│   ├── software_timer.h        # Software timer definitions
│   ├── main.h                  # Common application definitions
│   ├── stm32f1xx_hal_conf.h    # HAL configuration
│   ├── stm32f1xx_it.h          # Interrupt handler definitions
│
└── Startup/
    └── startup_stm32f103c6ux.s   # Startup assembly code

```
---
