#pragma once
#include QMK_KEYBOARD_H
#include <qp.h>
#include "animations.h"

const uint8_t * const gfx_images[] = {
    gfx_togepi_128x32,
    gfx_yibbygibby,
    gfx_bonggocat
};

enum qp_keycode {
    QP_NEXT = QK_KB_0,
    QP_PREV
};

#define NUM_IMAGES (sizeof(gfx_images) / sizeof(gfx_images[0]))

painter_device_t display;

static painter_image_handle_t my_image = NULL;
static deferred_token my_anim = INVALID_DEFERRED_TOKEN;
static uint8_t current_image_index = 0;
static uint16_t last_anim_switch = 0;

void eeconfig_init_user(void) {
    current_image_index = 0;
    eeconfig_update_user(current_image_index);
}

bool can_switch_anim(void) {
    uint16_t now = timer_read();
    if (timer_elapsed(last_anim_switch) > 250) {
        last_anim_switch = now;
        return true;
    }
    return false;
}

void load_animation(uint8_t index) {
    if (my_anim != INVALID_DEFERRED_TOKEN) {
        qp_stop_animation(my_anim);
        my_anim = INVALID_DEFERRED_TOKEN;
    }

    if (my_image != NULL) {
        qp_close_image(my_image);
        my_image = NULL;
    }

    my_image = qp_load_image_mem(gfx_images[index]);

    if (my_image) {
        my_anim = qp_animate(display, 0, 0, my_image);
        eeconfig_update_user(current_image_index);
    }
}

void keyboard_post_init_kb(void) {
    display = qp_sh1106_make_i2c_device(128, 32, 0x3c);
    qp_init(display, QP_ROTATION_180);
    qp_power(display, true);

    current_image_index = eeconfig_read_user();
    if (current_image_index >= NUM_IMAGES) {
        current_image_index = 0;
    }

    load_animation(current_image_index);
}

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
        case QP_NEXT:
            if(record->event.pressed && can_switch_anim()){
                current_image_index = (current_image_index + 1) % NUM_IMAGES;
                load_animation(current_image_index);
            }
            break;
        case QP_PREV:
            if(record->event.pressed && can_switch_anim()){
                if (current_image_index == 0) {
                    current_image_index = NUM_IMAGES - 1;
                } else {
                    current_image_index--;
                }
                load_animation(current_image_index);
            }
            break;
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
                        QP_PREV,   QP_NEXT,
        KC_1,   KC_2,   KC_3,   KC_4,
        KC_5,   KC_6,   KC_7,   KC_8,
        MO(1),   KC_9,   KC_0,   MO(2)
    ),

    [1] = LAYOUT(
                        KC_A,   KC_B,
        KC_C,   KC_D,   KC_E,   KC_F,
        KC_G,   KC_H,   KC_I,   KC_J,
        KC_TRNS,   KC_K,   KC_L,   MO(3)
    ),

    [2] = LAYOUT(
                        KC_M,   KC_N,
        KC_O,   KC_P,   KC_Q,   KC_R,
        KC_S,   KC_T,   KC_U,   KC_V,
        KC_TRNS,   KC_W,   KC_X,   KC_TRNS
    ),

    [3] = LAYOUT(
                              KC_TRNS,   QK_BOOTLOADER,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS
    )
};
