#include QMK_KEYBOARD_H

#include "../../../../../shared/secrets.h"

enum layers {
    BASE_LAYER,
    CAPS_LAYER,
    FN_LAYER,
    MAC_LAYER,
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
    /* Keymap _BL: (Base Layer) Default Layer
   * |------------------------------------------------------------------------------------------.
   * |  Esc  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |  Backsp  |
   * |------------------------------------------------------------------------------------------|
   * |  Tab    |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |   \    |
   * |------------------------------------------------------------------------------------------|
   * |  Fn1 / Caps  |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |  Enter  |
   * |------------------------------------------------------------------------------------------|
   * |  Shift / (    |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |   Shift / )  |
   * |------------------------------------------------------------------------------------------|
   * |  Ctrl  |  GUI  |  Alt  |              Space             |  Alt  |  Fn2  |  App  |  Ctrl  |
   * `------------------------------------------------------------------------------------------'
   */
    [BASE_LAYER] = LAYOUT_60_ansi(
        QK_GESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
        MO(CAPS_LAYER), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,
        SC_LSPO, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, SC_RSPC,
        KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_RALT, QK_LEAD, LT(FN_LAYER, KC_APP), KC_RCTL),
    /*
   * |----------------------------------------------------------------------------------------------------------------------------------------------------|
   * | Esc                      | F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10  | F11  | F12  | Backsp                                       |
   * |----------------------------------------------------------------------------------------------------------------------------------------------------|
   * |        | (Q)Prev | (W)Play | (E)Next |  (R)  |  (T) [  |  (Y) ]  |  (U)HOME  |  (I) ↑  |  (O)END  |  (P)PSCR  |  ([)PGUP  |  (])PGDN  |    (\)     |
   * |----------------------------------------------------------------------------------------------------------------------------------------------------|
   * |           | (A)Vol- | (S)Vol+ | (D)Mute | (F) |  (G) {  |  (H) }  |  (J) ←   |  (K) ↓  |  (L) → |  (;)  |  (')  |                                  |
   * |----------------------------------------------------------------------------------------------------------------------------------------------------|
   * |              |  (Z)   |  (X)  | (C)CAL |  (V)  |  (B)  |  (N)   |  (M) -  | (,) _  |  (.)   |  (/)  |                                              |
   * |----------------------------------------------------------------------------------------------------------------------------------------------------|
   * |       |       |       |                                                                                           |       |       |       |        |
   * |----------------------------------------------------------------------------------------------------------------------------------------------------|
   */
    [CAPS_LAYER] = LAYOUT_60_ansi(
        QK_GESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL,
        _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, KC_LBRC, KC_RBRC, KC_HOME, KC_UP, KC_END, KC_PSCR, KC_PGUP, KC_PGDN, _______,
        _______, KC_VOLD, KC_VOLU, KC_MUTE, _______, KC_LCBR, KC_RCBR, KC_LEFT, KC_DOWN, KC_RGHT, _______, KC_GRAVE, _______,
        _______, _______, _______, KC_CALC, _______, _______, KC_MINS, KC_UNDS, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______),
    /*
   * |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
   * |       |     |         |        |       |         |       |          |      |       |         |        |         |                                                  |
   * |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
   * |        | (Q)On/Off | (W)Mode | (E)Color+ |  (R)Color-  |  (T)Sat+  |  (Y)Sat-  |  (U)Bright+  |  (I)Bright-  | (O) |  (P) |  ([)  |  (])  |        RESET           |
   * |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
   * |           | (A) | (S) | (D) |  (F)  |  (G)  |  (H)   |  (J)   |  (K)Speed+   |  (L)Speed-  |  (;)  |  (')  |                      RGB Mode - Solid Color           |
   * |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
   * |              |  (Z)   |  (X)  | (C) |  (V)  |  (B)  |  (N)   |  (M)  | (,)  |  (.)   |  (/)  |                                                                     |
   * |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
   * |       |       |       |                                                                      |       |       |       |        |                                    |
   * |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
   */
    [FN_LAYER] = LAYOUT_60_ansi(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, RGB_TOG, _______, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______, _______, _______, QK_BOOT,
        _______, _______, _______, _______, _______, _______, _______, _______, RGB_SPI, RGB_SPD, _______, _______, RGB_M_P,
        KC_CAPS, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_CAPS,
        AG_TOGG, _______, _______, _______, _______, _______, _______, _______)
};
