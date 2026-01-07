# Macropads

This repository contains the full source files for my custom-designed mechanical macropads, featuring integrated OLED displays and programmable keys via QMK with VIAL support.

## 🛠 Project Overview

This project includes two distinct designs tailored for different productivity needs:
* **Hesu12**: A compact 12-key layout with an OLED screen.
* **Hesu14**: An expanded 14-key layout with an OLED screen.

---

## 🎹 Hardware Specifications

Both boards are designed with a focus on ease of assembly and high-performance hardware:

* **Controller Support**: Designed for Pro Micro or RP2040 Zero
* **Display**: I2C OLED modules (128x32).
* **Switches**: MX-compatible mechanical switch footprints.
* **Connectivity**: USB-C (via controller).

| Feature | Hesu12 | Hesu14 |
| :--- | :--- | :--- |
| **Key Count** | 12 Keys | 14 Keys |
| **Display** | 1x OLED | 1x OLED |
| **EDA Tool** | KiCad 8.0+ | KiCad 8.0+ |

---

## 🚀 Firmware Installation
These macropads utilize **QMK (Quantum Mechanical Keyboard)** firmware.

1. Clone the official [QMK Firmware](https://github.com/qmk/qmk_firmware) repository.
2. Copy the contents of the `/Firmware` directory into your local `qmk_firmware/keyboards/` folder.
3. Compile and flash your board using the QMK CLI:
