# 📟 Embedded C — ATMega328p Bare-Metal

A deep dive into embedded systems programming and bare-metal development. This project involves writing **C code from scratch** for the **ATMega328p** microcontroller (on a custom board) without any external libraries, communicating directly with the hardware via memory-mapped registers.

---

## 🛠️ Engineering Highlights

*   **Bare-Metal Development:** Zero external libraries or abstraction layers. Every peripheral is controlled by reading and writing directly to configuration registers.
*   **Hardware Protocols:** From-scratch implementation of low-level communication standards:
    *   **UART:** Serial communication for debugging and data transmission.
    *   **I2C (TWI):** Inter-Integrated Circuit protocol to interface with external sensors and displays.
*   **Timers & Interrupts:** Advanced configuration of hardware timers to manage precise delays, PWM signal generation, and hardware interrupt service routines (ISRs).

---

## 💾 Technical Specs

*   **Microcontroller:** ATMega328p
*   **Language:** Pure C
*   **Toolchain:** `avr-gcc`, `avrdude`, `make`
*   **Hardware Interaction:** Memory-mapped I/O registers

---

## 🚀 Compilation & Flashing

### Prerequisites
You need the AVR toolchain installed on your system:
```bash
# On Debian/Ubuntu
sudo apt-get install gcc-avr binutils-avr avr-libc avrdude
```

### Build & Flash

To compile the binary and upload it to the custom board via serial/ISP:
```bash
# Compile the project
make

# Flash the hex file to the ATMega328p
make flash
```
