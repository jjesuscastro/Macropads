#pragma once
#include QMK_KEYBOARD_H
#include <qp.h>
#include "fonts/arial.qff.h"
#include "animations.h"

/* ------------------------- User Config ------------------------- */
typedef union {
    uint32_t raw;
    struct {
        uint8_t current_image_index;
        bool is_using_AHK;
    };
} user_config_t;

user_config_t user_config;

/* ------------------------- Image Data ------------------------- */
const uint8_t * const gfx_images[] = {
    gfx_togepi_128x32,
    gfx_yibbygibby,
    gfx_bonggocat
};

#define NUM_IMAGES (sizeof(gfx_images) / sizeof(gfx_images[0]))

/* ------------------------- Custom Keycodes ------------------------- */
enum custom_keycodes {
    QP_NEXT = QK_KB_0,
    QP_PREV,
    TOG_AHK
};

/* ------------------------- Display / Animation ------------------------- */
painter_device_t display;
static painter_image_handle_t my_image = NULL;
static painter_font_handle_t font = NULL;
static deferred_token my_anim = INVALID_DEFERRED_TOKEN;
static deferred_token ahk_msg_token = INVALID_DEFERRED_TOKEN;
static bool showing_ahk_status = false;
static uint16_t last_anim_switch = 0;

/* ------------------------- Functions ------------------------- */
void eeconfig_init_user(void) {
    user_config.raw = 0;
    user_config.current_image_index = 0;
    user_config.is_using_AHK = false;
    eeconfig_update_user(user_config.raw);
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
        user_config.current_image_index = index;
        eeconfig_update_user(user_config.raw);
    }
}

uint32_t ahk_message_callback(uint32_t trigger_time, void *cb_arg) {
    showing_ahk_status = false;
    load_animation(user_config.current_image_index);
    return 0;
}

void show_status(const char *msg) {
    if (my_anim != INVALID_DEFERRED_TOKEN) {
        qp_stop_animation(my_anim);
        my_anim = INVALID_DEFERRED_TOKEN;
    }

    qp_clear(display);

    uint8_t text_width  = qp_textwidth(font, msg);
    uint8_t display_height = qp_get_height(display);

    int16_t x = (128-text_width) / 2;
    int16_t y = display_height / 2;

    qp_drawtext(display, x, y, font, msg);

    ahk_msg_token = defer_exec(2000, ahk_message_callback, NULL);
}

/* ------------------------- Init ------------------------- */
void keyboard_post_init_kb(void) {
    display = qp_sh1106_make_i2c_device(128, 32, 0x3c);
    qp_init(display, QP_ROTATION_180);
    qp_power(display, true);

    font = qp_load_font_mem(font_arial);

    user_config.raw = eeconfig_read_user();
    if (user_config.current_image_index >= NUM_IMAGES) {
        user_config.current_image_index = 0;
    }

    load_animation(user_config.current_image_index);
}

/* ------------------------- Key Processing ------------------------- */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MO(1):
            if (!user_config.is_using_AHK) break;
            if (record->event.pressed) {
                register_code(KC_F23);
            } else {
                unregister_code(KC_F23);
            }
            break;

        case MO(2):
            if (!user_config.is_using_AHK) break;
            if (record->event.pressed) {
                register_code(KC_F24);
            } else {
                unregister_code(KC_F24);
            }
            break;

        case QP_NEXT:
            if (record->event.pressed && can_switch_anim()) {
                user_config.current_image_index =
                    (user_config.current_image_index + 1) % NUM_IMAGES;
                load_animation(user_config.current_image_index);
            }
            break;

        case QP_PREV:
            if (record->event.pressed && can_switch_anim()) {
                if (user_config.current_image_index == 0) {
                    user_config.current_image_index = NUM_IMAGES - 1;
                } else {
                    user_config.current_image_index--;
                }
                load_animation(user_config.current_image_index);
            }
            break;

        case TOG_AHK:
            if (record->event.pressed) {
                user_config.is_using_AHK = !user_config.is_using_AHK;
                showing_ahk_status = true;
                
                if(!user_config.is_using_AHK) {
                    unregister_code(KC_F23);
                    unregister_code(KC_F24);
                }

                eeconfig_update_user(user_config.raw);
                show_status(user_config.is_using_AHK ? "AHK On" : "AHK Off");
            }
            break;
    }
    return true;
}

/* ------------------------- Keymaps ------------------------- */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /*
     * Layer 0 (Default) — 4 cols; OLED spans first 2 cols on row 1
     * ┌───────────────┬─────────┬─────────┐
     * │     OLED      │ QP_PREV │ QP_NEXT │
     * ├───────┬───────┼─────────┼─────────┤
     * │   1   │   2   │    3    │    4    │
     * ├───────┼───────┼─────────┼─────────┤
     * │   5   │   6   │    7    │    8    │
     * ├───────┼───────┼─────────┼─────────┤
     * │ MO(1) │   9   │    0    │  MO(2)  │
     * └───────┴───────┴─────────┴─────────┘
     */
    [0] = LAYOUT(
                        QP_PREV,   QP_NEXT,
        KC_1,   KC_2,   KC_3,   KC_4,
        KC_5,   KC_6,   KC_7,   KC_8,
        MO(1),  KC_9,   KC_0,   MO(2)
    ),

    /*
     * Layer 1 (FN1) — OLED spans first 2 cols on row 1
     * ┌───────────────┬─────────┬─────────┐
     * │     OLED      │    A    │    B    │
     * ├───────┬───────┼─────────┼─────────┤
     * │   C   │   D   │    E    │    F    │
     * ├───────┼───────┼─────────┼─────────┤
     * │   G   │   H   │    I    │    J    │
     * ├───────┼───────┼─────────┼─────────┤
     * │ TRNS  │   K   │    L    │  MO(3)  │
     * └───────┴───────┴─────────┴─────────┘
     */
    [1] = LAYOUT(
                        KC_A,   KC_B,
        KC_C,   KC_D,   KC_E,   KC_F,
        KC_G,   KC_H,   KC_I,   KC_J,
        KC_TRNS, KC_K,  KC_L,   MO(3)
    ),

    /*
     * Layer 2 (FN2) — OLED spans first 2 cols on row 1
     * ┌───────────────┬─────────┬─────────┐
     * │     OLED      │    M    │    N    │
     * ├───────┬───────┼─────────┼─────────┤
     * │   O   │   P   │    Q    │    R    │
     * ├───────┼───────┼─────────┼─────────┤
     * │   S   │   T   │    U    │    V    │
     * ├───────┼───────┼─────────┼─────────┤
     * │ TRNS  │   W   │    X    │  TRNS   │
     * └───────┴───────┴─────────┴─────────┘
     */
    [2] = LAYOUT(
                        KC_M,   KC_N,
        KC_O,   KC_P,   KC_Q,   KC_R,
        KC_S,   KC_T,   KC_U,   KC_V,
        KC_TRNS, KC_W,  KC_X,   KC_TRNS
    ),

    /*
     * Layer 3 (Bootloader) — OLED spans first 2 cols on row 1
     * ┌───────────────┬─────────┬───────────────┐
     * │     OLED      │  TRNS   │ QK_BOOTLOADER │
     * ├───────┬───────┼─────────┼───────────────┤
     * │ TRNS  │ TRNS  │  TRNS   │     TRNS      │
     * ├───────┼───────┼─────────┼───────────────┤
     * │ TRNS  │ TRNS  │  TRNS   │     TRNS      │
     * ├───────┼───────┼─────────┼───────────────┤
     * │ TRNS  │ TRNS  │  TRNS   │     TRNS      │
     * └───────┴───────┴─────────┴───────────────┘
     */
    [3] = LAYOUT(
                              KC_TRNS,   QK_BOOTLOADER,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS
    )
};