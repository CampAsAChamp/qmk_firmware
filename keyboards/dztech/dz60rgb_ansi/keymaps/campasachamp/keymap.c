#include QMK_KEYBOARD_H

#include "../../../../../users/campasachamp/campasachamp.h"
#include "../../../../../users/campasachamp/features/sentence_case.h"

// Notes
/*
    QK_GESC:
        - [Esc]     when pressed on its own
        - [~]       when pressed with Shift
        - [`]       when pressed with Win

    PC:     [L_Ctrl] [L_Win] [L_Alt/Option] [     ] [R_Alt/Option] [Leader] [Fn] [R_Ctrl]
    Mac:    [L_Ctrl] [L_Alt/Option] [L_Cmd] [     ] [R_Cmd] [Leader] [Fn] [R_Alt/Option]
*/

enum layers {
    BASE_LAYER,
    FN_LAYER,
    CAPS_LAYER,
    RGB_LAYER,
    MOUSE_LAYER
};

bool is_alt_tab_active = false; // ADD this near the beginning of keymap.c
uint16_t alt_tab_timer = 0;     // we will be using them soon.

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    // Activate auto sentence case
    if (!process_sentence_case(keycode, record)) { 
        return false; 
    }

    // Detect the activation of both Shifts
    if ((get_mods() & MOD_MASK_SHIFT) == MOD_MASK_SHIFT) {
        tap_code(KC_CAPS);  // Toggle Caps_lock values
    }

    switch (keycode) { // This will do most of the grunt work with the keycodes.
        case SUPER_ALT_TAB:
        if (record->event.pressed) {
            if (!is_alt_tab_active) {
                is_alt_tab_active = true;
                register_code(KC_LCTL);
            }
            alt_tab_timer = timer_read();
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
        }
        
        break;
    }

    // Keep processing like normal, don't do anything to prevent that
    return true;
};

void matrix_scan_user(void) { // The very important timer.
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 750) {
      unregister_code(KC_LCTL);
      is_alt_tab_active = false;
    }
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE_LAYER] = LAYOUT_60_ansi(
        QK_GESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
        LT(CAPS_LAYER, KC_ESC), LCMD_T(KC_A), LALT_T(KC_S), LCTL_T(KC_D), LSFT_T(KC_F), KC_G, KC_H, RSFT_T(KC_J), LCTL_T(KC_K), LOPT_T(KC_L), LCMD_T(KC_SCLN), KC_QUOT, KC_ENT,
        TD(TD_CAPS), MY_MEH_Z, MY_HYPER_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, MT(MOD_RCTL, KC_SLASH), TD(TD_CAPS),
        KC_LCTL, KC_LWIN, KC_LALT, LT(FN_LAYER, KC_SPC), MO(MOUSE_LAYER), LT(RGB_LAYER, KC_APP), _______, KC_RCTL),

    [FN_LAYER] = LAYOUT_60_ansi(
        LCTL(KC_GRV), KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL,
        SUPER_ALT_TAB, LCTL(KC_Q), LCTL(KC_W), LCTL(KC_E), LCTL(KC_R), KC_LBRC, KC_RBRC, KC_HOME, KC_UP, KC_END, KC_PSCR, KC_PGUP, KC_PGDN, _______,
        _______, _______, _______, _______, _______, KC_LCBR, KC_RCBR, KC_LEFT, KC_DOWN, KC_RGHT, _______, KC_GRAVE, LCTL(KC_ENT),
        _______, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), LCTL(KC_B), KC_MINS, KC_UNDS, LCTL(KC_COMM), _______, LCTL(KC_SLASH), _______,
        _______, _______, _______, LCTL(KC_SPC), _______, _______, _______, _______),

    [CAPS_LAYER] = LAYOUT_60_ansi(
        KC_TILD, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL,
        _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, KC_LBRC, KC_RBRC, _______, _______, _______, KC_PSCR, KC_PGUP, KC_PGDN, _______,
        _______, KC_VOLD, KC_VOLU, _______, _______, KC_LCBR, KC_RCBR, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______),
        
    [RGB_LAYER] = LAYOUT_60_ansi(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_M_P,
        KC_CAPS, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, RGB_SPI, RGB_SPD, KC_CAPS,
        _______, _______, _______, _______, _______, _______, _______, _______),

    [MOUSE_LAYER] = LAYOUT_60_ansi(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, KC_WH_U, KC_MS_U, KC_WH_D, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, KC_MS_BTN2, KC_MS_BTN1, _______, _______, _______, _______)
};
