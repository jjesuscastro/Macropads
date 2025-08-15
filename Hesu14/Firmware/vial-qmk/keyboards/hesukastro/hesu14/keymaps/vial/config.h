#pragma once
#define RP2040_MCUCONF

// Vial
#define VIAL_KEYBOARD_UID {0x72, 0x70, 0x4A, 0xE2, 0x2B, 0x79, 0x5C, 0x60}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 2, 3 }

// Bootmagic
#define BOOTMAGIC_ROW 0
#define BOOTMAGIC_COLUMN 2

// OLED
#define I2C_DRIVER I2CD0
#define I2C1_SCL_PIN GP1
#define I2C1_SDA_PIN GP0

#define OLED_DISPLAY_128X32
#define OLED_TIMEOUT 0