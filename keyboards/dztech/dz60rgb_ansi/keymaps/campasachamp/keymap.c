#include QMK_KEYBOARD_H

#include "../../../../../shared/secrets.h"

enum layers {
    BASE_LAYER,
    CAPS_LAYER,
    FN_LAYER,
    RGB_LAYER,
    MOUSE_LAYER
};

// Notes
/*
    QK_GESC:
        - [Esc]     when pressed on its own
        - [~]       when pressed with Shift
        - [`]       when pressed with Win

    PC:     [L_Ctrl] [L_Win] [L_Alt/Option] [     ] [R_Alt/Option] [Leader] [Fn] [R_Ctrl]
    Mac:    [L_Ctrl] [L_Alt/Option] [L_Cmd] [     ] [R_Cmd] [Leader] [Fn] [R_Alt/Option]
*/

bool did_leader_succeed;

void leader_end_user(void){
    did_leader_succeed = false;

    if (leader_sequence_one_key(KC_C)){
        send_string(USERNAME);
    }
    else if (leader_sequence_two_keys(KC_N, KC_P)){
        send_string(P_EMAIL);
    } 
    else if (leader_sequence_two_keys(KC_N, KC_W)){
        send_string(W_EMAIL);
    } 
    else if (leader_sequence_two_keys(KC_N, KC_U)){
        send_string(S_EMAIL);
    }
    else if (leader_sequence_one_key(KC_V)){
        send_string(V);
    }
    else if (leader_sequence_one_key(KC_1)){
        send_string(MEGA);
    }
    else if (leader_sequence_one_key(KC_4)){
        send_string(FOUR);
    }
    else if (leader_sequence_two_keys(KC_4, KC_T)){
        send_string(FOUR_CAPS);
    }
    else if (leader_sequence_two_keys(KC_4, KC_4)){
        send_string(FOUR_SYM);
    }
    else if (leader_sequence_one_key(KC_3)){
        send_string(PHONE_NUM);
    }
    else if (leader_sequence_two_keys(KC_3, KC_TAB)){
        send_string(PHONE_NUM_TAB);
    }
    else if (leader_sequence_two_keys(KC_M, KC_P)){
        send_string(M);
    }
    else if (leader_sequence_three_keys(KC_M, KC_P, KC_P)){
        send_string(M_SYM);
    }
    else if (leader_sequence_one_key(QK_GESC)){
        // Sends Ctrl + Shift + Esc (Open Task Manager)
        // Need this because QK_GESC sends Grave(`) or Tilde(~) if holding Ctrl or Shift and pressing enter
        // Holding Ctrl + Shift and pressing Esc will give us a Tilde instead of pressing the key combo
        send_string(SS_DOWN(X_LCTL) SS_DOWN(X_LSFT) SS_TAP(X_ESC) SS_UP(X_LSFT) SS_UP(X_LCTL));
    }

    // Pressing the leader key and not pressing any keys after will allow us to send the actual App key press
    if (!did_leader_succeed){
        send_string(SS_TAP(X_APP));
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    // Detect the activation of both Shifts
    if ((get_mods() & MOD_MASK_SHIFT) == MOD_MASK_SHIFT) {
        // Toggle Caps_lock values
        tap_code(KC_CAPS);
    }
 
    // Keep processing like normal, don't do anything to prevent that
    return true;
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE_LAYER] = LAYOUT_60_ansi(
        QK_GESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
        MO(CAPS_LAYER), LCTL_T(KC_A), LCMD_T(KC_S), LSFT_T(KC_D), LOPT_T(KC_F), KC_G, KC_H, LOPT_T(KC_J), RSFT_T(KC_K), LCMD_T(KC_L), LCTL_T(KC_SCLN), KC_QUOT, KC_ENT,
        SC_LSPO, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, SC_RSPC,
        KC_LCTL, KC_LGUI, MO(FN_LAYER), KC_SPC, MO(MOUSE_LAYER), QK_LEAD, LT(RGB_LAYER, KC_APP), KC_RCTL),

    [CAPS_LAYER] = LAYOUT_60_ansi(
        _______, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL,
        _______, _______, _______, _______, _______, KC_LBRC, KC_RBRC, KC_HOME, KC_UP, KC_END, KC_PSCR, KC_PGUP, KC_PGDN, _______,
        _______, _______, _______, _______, _______, KC_LCBR, KC_RCBR, KC_LEFT, KC_DOWN, KC_RGHT, _______, KC_GRAVE, _______,
        _______, _______, _______, _______, _______, _______, KC_MINS, KC_UNDS, _______, _______, _______, _______,
        _______, _______, _______, CW_TOGG, _______, _______, _______, _______),

    [FN_LAYER] = LAYOUT_60_ansi(
        LOPT(KC_GRV), KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL,
        LOPT(KC_TAB), LOPT(KC_Q), LOPT(KC_W), LOPT(KC_E), LOPT(KC_R), KC_LBRC, KC_RBRC, KC_HOME, KC_UP, KC_END, KC_PSCR, KC_PGUP, KC_PGDN, _______,
        _______, LOPT(KC_A), LOPT(KC_S), LOPT(KC_D), LOPT(KC_F), KC_LCBR, KC_RCBR, KC_LEFT, KC_DOWN, KC_RGHT, _______, KC_GRAVE, _______,
        _______, LOPT(KC_Z), LOPT(KC_X), LOPT(KC_C), LOPT(KC_V), LOPT(KC_B), KC_MINS, KC_UNDS, _______, _______, _______, _______,
        _______, _______, _______, CW_TOGG, _______, _______, _______, _______),

    [RGB_LAYER] = LAYOUT_60_ansi(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT,
        _______, _______, KC_VOLD, KC_VOLU, _______, _______, _______, _______, _______, _______, _______, _______, RGB_M_P,
        KC_CAPS, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, RGB_SPI, RGB_SPD, KC_CAPS,
        AG_TOGG, _______, _______, _______, _______, _______, _______, _______),

    [MOUSE_LAYER] = LAYOUT_60_ansi(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, KC_MS_U, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, KC_MS_L, KC_MS_D, KC_MS_U, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______)
};
