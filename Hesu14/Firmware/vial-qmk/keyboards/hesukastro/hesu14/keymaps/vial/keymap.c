#include QMK_KEYBOARD_H
#include "frog.c"

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

bool oled_task_user(void) {

    render_animation();
    // oled_write_raw_P(image, sizeof(image));
    return false;
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case MO(1):
            if(record->event.pressed) {
                register_code(KC_F23);
            } else {
                unregister_code(KC_F23);
            }
            break;
        case MO(2):
            if(record->event.pressed) {
                register_code(KC_F24);
            } else {
                unregister_code(KC_F24);
            }
    }
    return true;
}

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
