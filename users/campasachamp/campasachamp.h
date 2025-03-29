#ifndef CAMPASACHAMP_H
#define CAMPASACHAMP_H

#include "quantum.h"

void dance_caps (tap_dance_state_t *state, void *user_data);

void dance_l_bracket (tap_dance_state_t *state, void *user_data);

void dance_r_bracket (tap_dance_state_t *state, void *user_data);

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
    COLON_EQUAL,
};

#endif
