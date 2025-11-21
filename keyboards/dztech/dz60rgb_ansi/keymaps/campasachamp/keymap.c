#include QMK_KEYBOARD_H

// Notes
/*
    QK_GESC:
        - [Esc]     when pressed on its own
        - [~]       when pressed with Shift
        - [`]       when pressed with Win

    PC:     [L_Ctrl] [L_Win] [L_Alt/Option] [     ] [R_Alt/Option] [Leader] [Fn] [R_Ctrl]
    Mac:    [L_Ctrl] [L_Alt/Option] [L_Cmd] [     ] [R_Cmd] [Leader] [Fn] [R_Alt/Option]
*/
#define MY_HYPER S(G(C(KC_LALT)))
#define MY_MEH S(G(KC_LALT))

#define MY_HYPER_X MT(MOD_LSFT | MOD_LGUI | MOD_LCTL | MOD_LALT, KC_X)
#define MY_MEH_Z MT(MOD_LSFT | MOD_LGUI | MOD_LALT, KC_Z)
#define MY_HYPER_DOT MT(MOD_LSFT | MOD_LGUI | MOD_LCTL | MOD_LALT, KC_DOT)

enum layers {
    BASE_LAYER,
    SHORTCUT_LAYER,
    SYMBOL_LAYER,
    MEDIA_LAYER,
    RGB_LAYER,
    MOUSE_LAYER
};

// Tap Dance index enum declarations
enum td_keycodes {
    TD_DASH,
    TD_PLUS,
    TD_LBRACKET,
    TD_RBRACKET,
    TD_PIPE,
    TD_COPY,
    TD_UNDO,
    TD_CAPS,
};

enum custom_keycodes {
    SUPER_ALT_TAB = SAFE_RANGE,
    RGB_SLD,
    COLON_EQUAL,
};

void dance_caps(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        set_oneshot_mods(MOD_BIT(KC_LSFT));
    } else if (state->count == 2) {
        caps_word_on();
    } else if (state->count == 3) {
        tap_code(KC_CAPS);
    } else {
        reset_tap_dance(state);
    }
}

void dance_l_bracket(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        // Left parenthesis -- (
        register_code(KC_LSFT);
        tap_code(KC_9);
        unregister_code(KC_LSFT);
    } else if (state->count == 2) {
        // Left Curly Bracket -- {
        register_code(KC_LSFT);
        tap_code(KC_LEFT_BRACKET);
        unregister_code(KC_LSFT);
    } else if (state->count == 3) {
        // Left Square Bracket -- [
        tap_code(KC_LEFT_BRACKET);
    } else if (state->count == 4) {
        // Left Angle Bracket -- <
        register_code(KC_LSFT);
        tap_code(KC_COMMA);
        unregister_code(KC_LSFT);
    } else {
        reset_tap_dance(state);
    }
}

void dance_r_bracket(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        // Right parenthesis -- )
        register_code(KC_LSFT);
        tap_code(KC_0);
        unregister_code(KC_LSFT);
    } else if (state->count == 2) {
        // Right Curly Bracket -- }
        register_code(KC_LSFT);
        tap_code(KC_RIGHT_BRACKET);
        unregister_code(KC_LSFT);
    } else if (state->count == 3) {
        // Right Square Bracket -- ]
        tap_code(KC_RIGHT_BRACKET);
    } else if (state->count == 4) {
        // Right Angle Bracket -- >
        register_code(KC_LSFT);
        tap_code(KC_DOT);
        unregister_code(KC_LSFT);
    } else {
        reset_tap_dance(state);
    }
}

tap_dance_action_t tap_dance_actions[] = {
    // Tap once for first key code tap twice for second keycode
    [TD_DASH] = ACTION_TAP_DANCE_DOUBLE(KC_MINUS, KC_UNDS), 
    [TD_PLUS] = ACTION_TAP_DANCE_DOUBLE(KC_EQUAL, KC_PLUS), 
    [TD_LBRACKET] = ACTION_TAP_DANCE_FN(dance_l_bracket), 
    [TD_RBRACKET] = ACTION_TAP_DANCE_FN(dance_r_bracket), 
    [TD_PIPE] = ACTION_TAP_DANCE_DOUBLE(KC_BACKSLASH, KC_PIPE), 
    [TD_COPY] = ACTION_TAP_DANCE_DOUBLE(LGUI(KC_C), LGUI(KC_X)), 
    [TD_UNDO] = ACTION_TAP_DANCE_DOUBLE(LGUI(KC_Z), LGUI(LSFT(KC_Z))), 
    [TD_CAPS] = ACTION_TAP_DANCE_FN(dance_caps),
};

bool is_alt_tab_active = false; // ADD this near the beginning of keymap.c
uint16_t alt_tab_timer = 0;     // we will be using them soon.

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    // Activate auto sentence case
    // if (!process_sentence_case(keycode, record)) { 
    //     return false; 
    // }

    // Detect the activation of both Shifts
    // if ((get_mods() & MOD_MASK_SHIFT) == MOD_MASK_SHIFT) {
    //     tap_code(KC_CAPS);  // Toggle Caps_lock values
    // }

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
        LT(MEDIA_LAYER, KC_ESC), LCTL_T(KC_A), LALT_T(KC_S), LCMD_T(KC_D), LSFT_T(KC_F), KC_G, KC_H, RSFT_T(KC_J), LCMD_T(KC_K), LOPT_T(KC_L), LCTL_T(KC_SCLN), KC_QUOT, KC_ENT,
        TD(TD_CAPS), MY_MEH_Z, MY_HYPER_X, KC_C, KC_V, KC_B, KC_N, KC_M, LT(SYMBOL_LAYER, KC_COMMA), MY_HYPER_DOT, MT(MOD_RCTL, KC_SLASH), TD(TD_CAPS),
        KC_LALT, KC_LWIN, KC_LCTL, LT(SHORTCUT_LAYER, KC_SPC), MO(MOUSE_LAYER), LT(RGB_LAYER, KC_APP), _______, KC_RCTL),

    [SHORTCUT_LAYER] = LAYOUT_60_ansi(
        LCTL(KC_GRV), KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, LALT(KC_BSPC),
        SUPER_ALT_TAB, LCTL(KC_Q), LCTL(KC_W), LCTL(KC_E), LCTL(KC_R), _______, _______, KC_HOME, KC_UP, KC_END, KC_PSCR, KC_PGUP, KC_PGDN, _______,
        _______, _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, KC_GRAVE, LCTL(KC_ENT),
        _______, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), LCTL(KC_B), KC_MINS, KC_UNDS, LCTL(KC_COMM), _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______),

    [SYMBOL_LAYER] = LAYOUT_60_ansi(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, KC_LCBR, KC_RCBR, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, KC_LPRN, KC_RPRN, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, KC_LBRC, KC_RBRC, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______),

    [MEDIA_LAYER] = LAYOUT_60_ansi(
        KC_TILD, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL,
        _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, _______, _______, _______, _______, _______, KC_PSCR, KC_PGUP, KC_PGDN, _______,
        _______, KC_VOLD, KC_VOLU, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______),
        
    [RGB_LAYER] = LAYOUT_60_ansi(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_M_P,
        KC_CAPS, RM_TOGG, RM_NEXT, RM_HUEU, RM_HUED, RM_SATU, RM_SATD, RM_VALU, RM_VALD, RM_SPDU, RM_SPDU, KC_CAPS,
        _______, _______, _______, _______, _______, _______, _______, _______),

    [MOUSE_LAYER] = LAYOUT_60_ansi(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, MS_WHLU, MS_UP, MS_WHLD, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT,
        _______, MS_WHLL, MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLL, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, MS_BTN2, MS_BTN1, _______, _______, _______, _______)
};


/*
BLANK_LAYER_TEMPLATE
----------------------
    [REPLACE_ME_LAYER] = LAYOUT_60_ansi(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______),

*/
