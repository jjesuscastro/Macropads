// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┐
     * │ OLED  │ 1 │ 2 │
     * ├───┼───┼───┼───┤
     * │ 3 │ 4 │ 5 │ 6 │
     * ├───┼───┼───┼───┤
     * │ 7 │ 8 │ 9 │ 0 │
     * ├───┼───┼───┼───┤
     * │ B │ B │ C │ D │
     * └───┴───┴───┴───┘
     */
    [0] = LAYOUT(
                        KC_1,   KC_2,
        KC_3,   KC_4,   KC_5,   KC_6,
        KC_7,   KC_8,   KC_9,   KC_0,
        KC_A,   KC_B,   KC_C,   KC_D
    )
};
