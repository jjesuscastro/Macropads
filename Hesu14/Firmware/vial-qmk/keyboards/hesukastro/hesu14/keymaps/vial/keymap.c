#include QMK_KEYBOARD_H
#include <qp.h>
#include "animation.qgf.h"
#include "animation_2.qgf.h"

#ifdef QUANTUM_PAINTER_ENABLE
painter_device_t display;

static painter_image_handle_t my_image = NULL;
static deferred_token my_anim;

void keyboard_post_init_kb(void) {
    display = qp_sh1106_make_i2c_device(128, 32, 0x3c);
    qp_init(display, QP_ROTATION_180);   // Initialise the display
    qp_power(display, true);

    my_image = qp_load_image_mem(gfx_animation_2);
    if (my_image != NULL) {
        my_anim = qp_animate(display, 0, 0, my_image);
    }
}

// void housekeeping_task_user(void) {
//     static uint32_t last_draw = 0;
//     if (timer_elapsed32(last_draw) > 33) { // Throttle to 30fps
//         last_draw = timer_read32();
//         // Draw 8px-wide rainbow down the left side of the display
//         for (int i = 0; i < 239; ++i) {
//             qp_line(display, 0, i, 7, i, i, 255, 255);
//         }
//         qp_flush(display);
//     }
// }
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
