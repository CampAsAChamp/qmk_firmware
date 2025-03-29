#include "campasachamp.h"

void dance_caps (tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        set_oneshot_mods(MOD_BIT(KC_LSFT));
    } else if (state->count == 2) {
        caps_word_on();
    } else if (state->count == 3) {
        tap_code(KC_CAPS);
    } else {
        reset_tap_dance (state);
    }
}

void dance_l_bracket (tap_dance_state_t *state, void *user_data) {
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
       reset_tap_dance (state);
   }
}

void dance_r_bracket (tap_dance_state_t *state, void *user_data) {
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
       reset_tap_dance (state);
   }
}

tap_dance_action_t tap_dance_actions[] = {
    // Tap once for first key code tap twice for second keycode
    [TD_DASH]     = ACTION_TAP_DANCE_DOUBLE(KC_MINUS, KC_UNDS),
    [TD_PLUS]     = ACTION_TAP_DANCE_DOUBLE(KC_EQUAL, KC_PLUS),
    [TD_LBRACKET] = ACTION_TAP_DANCE_FN(dance_l_bracket),
    [TD_RBRACKET] = ACTION_TAP_DANCE_FN(dance_r_bracket),
    [TD_PIPE]     = ACTION_TAP_DANCE_DOUBLE(KC_BACKSLASH, KC_PIPE),
    [TD_COPY]     = ACTION_TAP_DANCE_DOUBLE(LGUI(KC_C), LGUI(KC_X)),
    [TD_UNDO]     = ACTION_TAP_DANCE_DOUBLE(LGUI(KC_Z), LGUI(LSFT(KC_Z))),
    [TD_CAPS]     = ACTION_TAP_DANCE_FN(dance_caps),
};
