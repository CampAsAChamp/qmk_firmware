#include QMK_KEYBOARD_H

#include "./key_indexes.h"
// #include "../../../users/campasachamp/campasachamp.h"
// #include "../../../users/campasachamp/features/sentence_case.h"
#define MY_HYPER S(G(C(KC_LALT)))
#define MY_MEH S(G(KC_LALT))

#define MY_HYPER_X MT(MOD_LSFT | MOD_LGUI | MOD_LCTL | MOD_LALT, KC_X)
#define MY_MEH_Z MT(MOD_LSFT | MOD_LGUI | MOD_LALT, KC_Z)
#define MY_HYPER_DOT MT(MOD_LSFT | MOD_LGUI | MOD_LCTL | MOD_LALT, KC_DOT)

enum layers {
    BASE, // default layer
    SHORTCUTS,
    SYMBOLS,
    MEDIA,
    NUMBERS,
    MOUSE,
    GAMING,
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
    [TD_DASH] = ACTION_TAP_DANCE_DOUBLE(KC_MINUS, KC_UNDS), [TD_PLUS] = ACTION_TAP_DANCE_DOUBLE(KC_EQUAL, KC_PLUS), [TD_LBRACKET] = ACTION_TAP_DANCE_FN(dance_l_bracket), [TD_RBRACKET] = ACTION_TAP_DANCE_FN(dance_r_bracket), [TD_PIPE] = ACTION_TAP_DANCE_DOUBLE(KC_BACKSLASH, KC_PIPE), [TD_COPY] = ACTION_TAP_DANCE_DOUBLE(LGUI(KC_C), LGUI(KC_X)), [TD_UNDO] = ACTION_TAP_DANCE_DOUBLE(LGUI(KC_Z), LGUI(LSFT(KC_Z))), [TD_CAPS] = ACTION_TAP_DANCE_FN(dance_caps),
};

// State Variables
static bool is_leader_key_pressed = false;
bool        is_alt_tab_active     = false;
uint16_t    alt_tab_timer         = 0;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /*
     * ,--------------------------------------------------.    ,--------------------------------------------------.
     * |    0   |   1  |   2  |   3  |   4  |   5  |  6   |    |  38  |  39  |  40  |  41  |  42  |  43  |   44   |
     * |--------+------+------+------+------+------+------|    |------+------+------+------+------+------+--------|
     * |    7   |   8  |   9  |  10  |  11  |  12  |  13  |    |  45  |  46  |  47  |  48  |  49  |  50  |   51   |
     * |--------+------+------+------+------+------|      |    |      |------+------+------+------+------+--------|
     * |   14   |  15  |  16  |  17  |  18  |  19  |------|    |------|  52  |  53  |  54  |  55  |  56  |   57   |
     * |--------+------+------+------+------+------|  26  |    |  58  |------+------+------+------+------+--------|
     * |   20   |  21  |  22  |  23  |  24  |  25  |      |    |      |  59  |  60  |  61  |  62  |  63  |   64   |
     * `--------+------+------+------+------+-------------'    `-------------+------+------+------+------+--------'
     *   |  27  |  28  |  29  |  30  |  31  |                                |  65  |  66  |  67  |  68  |  69  |
     *   `----------------------------------'                                `----------------------------------'
     *                                       ,-------------.  ,-------------.
     *                                       |  32  |  33  |  |  70  |  71  |
     *                                ,------+------+------|  |------+------+------.
     *                                |      |      |  34  |  |  72  |      |      |
     *                                |  35  |  36  |------|  |------|  74  |  75  |
     *                                |      |      |  37  |  |  73  |      |      |
     *                                `--------------------'  `--------------------'
     */

[BASE] = LAYOUT_ergodox_pretty(
  KC_MINUS,         KC_1,         KC_2,         KC_3,         KC_4,           KC_5,         _______,      _______            , KC_6       , KC_7        , KC_8        , KC_9        , KC_0                , KC_EQUAL    ,
  KC_TAB,         KC_Q,         KC_W,         KC_E,         KC_R,           KC_T,  _______,      _______     , KC_Y       , KC_U        , KC_I        , KC_O        , KC_P                , TD(TD_PIPE),
  LT(MEDIA, KC_ESC), LCMD_T(KC_A), LALT_T(KC_S), LCTL_T(KC_D), LSFT_T(KC_F),           KC_G,                                              KC_H       , RSFT_T(KC_J), LCTL_T(KC_K), LALT_T(KC_L), LCMD_T(KC_SEMICOLON), KC_QUOTE   ,
  TD(TD_CAPS) ,         MY_MEH_Z,         MY_HYPER_X,          LT(SYMBOLS, KC_C),        KC_V,           KC_B,   LCTL(KC_SPACE),      TD(TD_CAPS)         , KC_N       , KC_M        , LT(SYMBOLS, KC_COMMA)    , MY_HYPER_DOT      , MT(MOD_RCTL, KC_SLASH)            , TD(TD_CAPS)     ,
  TOGGLE_LAYER_COLOR,       _______,      _______, _______,  MO(SHORTCUTS),                                                                           TT(MOUSE)   , _______     , _______     , _______             , _______    ,

                                                                   LALT(KC_SPACE), LCTL(LSFT(KC_1)),      TG(GAMING)          , TT(NUMBERS),
                                                                                   LCTL(LSFT(KC_2)),      _______             ,
                                                         KC_SPACE,      KC_DELETE, LCTL(LSFT(KC_5)),      LCMD(LCTL(KC_SPACE)), KC_ENTER   , KC_BSPC
),

[SHORTCUTS] = LAYOUT_ergodox_pretty(
     LCTL(KC_GRV),      KC_F1,      KC_F2,      KC_F3,      KC_F4,   KC_F5,  KC_F11,      KC_F12 , KC_F6         , KC_F7    , KC_F8  , KC_F9   , KC_F10        , TO(BASE),
     SUPER_ALT_TAB, LCTL(KC_Q), LCTL(KC_W),    _______,    _______, KC_LBRC, _______,      _______, KC_RBRC       , KC_HOME  , KC_UP  , KC_END  , _______       , _______ ,
     LCTL(KC_GRV),    _______,    _______,    _______,    _______, KC_LCBR,                        KC_RCBR       , KC_LEFT  , KC_DOWN, KC_RIGHT, _______       , KC_GRAVE,
     _______, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), _______, KC_CAPS,      TD(TD_CAPS), KC_MINS       , KC_UNDS  , _______, _______ , _______, _______ ,
     _______,    _______,    _______,    _______,    _______,                                                 _______  , _______, _______ , _______       , _______ ,

                                                              _______, _______,      _______, _______       ,
                                                                       _______,      _______,
                                                     _______, _______, QK_LEAD,      _______, LCTL(KC_ENTER), LALT(KC_BSPC)
),

[SYMBOLS] = LAYOUT_ergodox_pretty(
  _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
  _______, _______, _______, KC_LCBR, KC_RCBR, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
  _______, _______, _______, KC_LPRN, KC_RPRN, _______,                       _______, _______, _______, _______, _______, _______,
  _______, _______, _______, KC_LBRC, KC_RBRC, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______,                                         _______, _______, _______, _______, _______,

                                               _______, _______,     _______, _______,
                                                        _______,     _______,
                                      _______, _______, _______,     _______, _______, _______
),

[MEDIA] = LAYOUT_ergodox_pretty(
LCTL(KC_GRV),      KC_F1,      KC_F2,      KC_F3,      KC_F4,   KC_F5,  KC_F11,      KC_F12 , KC_F6         , KC_F7    , KC_F8  , KC_F9   , KC_F10        , TO(BASE),
_______, _______, KC_MPRV,  KC_MPLY, KC_MNXT, _______,            _______,      _______, _______, _______, _______, _______, _______, _______ ,
_______, _______, KC_VOLD,  KC_VOLU, _______, _______,                                   _______, _______, _______, _______, _______, _______ ,
_______, _______, _______, _______,  _______, _______,            _______,      _______, _______, _______, _______, _______, _______, _______ ,
_______, _______, _______, _______, _______,                                                     _______, _______, _______, _______, _______ ,

                                              RGB_TOG,     RGB_MODE_PLAIN,      _______, _______,
                                                       TOGGLE_LAYER_COLOR,      _______,
                                     _______, _______,            _______,      _______, _______, _______
),

[NUMBERS] = LAYOUT_ergodox_pretty(
_______, _______, _______, _______,  _______, _______, _______,      _______, _______, _______, _______ , _______, _______, TO(BASE),
_______, _______, _______,   KC_UP,  _______, _______, _______,      _______, _______, KC_7   , KC_8    , KC_9   , KC_ASTR, _______ ,
_______, _______, KC_LEFT, KC_DOWN, KC_RIGHT, _______,                        _______, KC_4   , KC_5    , KC_6   , KC_PLUS, _______ ,
_______, _______, _______, _______,  _______, _______, _______,      _______, KC_AMPR, KC_1   , KC_2    , KC_3   , KC_BSLS, _______ ,
_______, _______, _______, _______,  _______,                                          KC_0   , KC_COMMA, KC_DOT , KC_EQL , _______ ,

                                              _______, _______,      XXXXXXX, _______,
                                                       _______,      _______,
                                     _______, _______, _______,      _______, _______, _______
),

[MOUSE] = LAYOUT_ergodox_pretty(
_______, KC_ACL0, KC_ACL1, KC_ACL2, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______,
_______, _______, KC_WH_U, KC_MS_U, KC_WH_D, _______, _______,      _______, _______, KC_WH_U, KC_MS_U, KC_WH_D, _______, QK_BOOT,
_______, KC_WH_L, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_R,                        KC_WH_L, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_R, _______,
KC_BTN4, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, KC_BTN5,
_______, _______, _______, _______, KC_BTN1,                                          _______, _______, _______, _______, _______,

                                             _______, _______,      _______, _______,
                                                      _______,      _______,
                                    KC_BTN2, KC_BTN3, _______,      _______, _______, _______
),

[GAMING] = LAYOUT_ergodox_pretty(
_______, _______, _______, _______,  _______, _______,     _______,      _______, _______, _______, _______, _______, _______     , TO(BASE),
_______, _______, _______, _______,  _______, _______,     _______,      _______, _______, _______, _______, _______, _______     , _______ ,
_______,    KC_A,    KC_S,    KC_D,     KC_F, _______,                            _______, KC_J   , KC_K   , KC_L   , KC_SEMICOLON, _______ ,
KC_LSFT, _______, _______, _______,  _______, _______, MO(SYMBOLS),      _______, _______, _______, _______, _______, _______     , KC_RSFT ,
KC_LCTL, KC_LALT, KC_LWIN, XXXXXXX, KC_SPACE,                                              _______, KC_LEFT, KC_UP  , KC_DOWN     , KC_RIGHT,

                                              _______,     _______,      _______, XXXXXXX,
                                                           _______,      _______,
                                     _______, _______,     _______,      _______, _______, _______
)

};


//----------------------
// BLANK LAYER TEMPLATE
//----------------------
 /*
[REPLACE_ME] = LAYOUT_ergodox_pretty(
  _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______,                       _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______,                                         _______, _______, _______, _______, _______,

                                               _______, _______,     _______, _______,
                                                        _______,     _______,
                                      _______, _______, _______,     _______, _______, _______
),
*/

extern rgb_config_t rgb_matrix_config;

// void keyboard_post_init_user(void) {
//   rgb_matrix_enable();
// }

const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
    [0] = { {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255} },

    [1] = { {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {131,255,255}, {41,255,255}, {131,255,255}, {0,245,245}, {0,245,245}, {41,255,255}, {41,255,255}, {41,255,255}, {0,245,245}, {131,255,255}, {131,255,255}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {131,255,255}, {131,255,255}, {131,255,255}, {0,245,245}, {0,245,245}, {131,255,255}, {131,255,255}, {131,255,255}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245} },

    [2] = { {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {219,255,255}, {25,255,255}, {219,255,255}, {131,255,255}, {188,255,255}, {25,255,255}, {25,255,255}, {25,255,255}, {188,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {74,213,161}, {219,255,255}, {25,255,255}, {219,255,255}, {131,255,255}, {188,255,255}, {25,255,255}, {25,255,255}, {25,255,255}, {188,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {131,255,255}, {25,255,255}, {131,255,255}, {131,255,255}, {131,255,255} },

    [3] = { {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {74,255,255}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {74,255,255}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255} },

    [4] = { {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,245,245}, {0,0,255}, {0,245,245}, {0,245,245}, {0,245,245} },

    [5] = { {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193}, {34,215,193} },

};

void set_layer_color(int layer) {
  for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
    HSV hsv = {
      .h = pgm_read_byte(&ledmap[layer][i][0]),
      .s = pgm_read_byte(&ledmap[layer][i][1]),
      .v = pgm_read_byte(&ledmap[layer][i][2]),
    };
    if (!hsv.h && !hsv.s && !hsv.v) {
        rgb_matrix_set_color( i, 0, 0, 0 );
    } else {
        RGB rgb = hsv_to_rgb( hsv );
        float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
        rgb_matrix_set_color( i, f * rgb.r, f * rgb.g, f * rgb.b );
    }
  }
}

bool rgb_matrix_indicators_user(void) {
  if (keyboard_config.disable_layer_led) { return false; }
  switch (biton32(layer_state)) {
    case 0:
      set_layer_color(0);
      break;
    case 1:
      set_layer_color(1);
      break;
    case 2:
      set_layer_color(2);
      break;
    case 3:
      set_layer_color(3);
      break;
    case 4:
      set_layer_color(4);
      break;
    case 5:
      set_layer_color(5);
      break;
   default:
    if (rgb_matrix_get_flags() == LED_FLAG_NONE)
      rgb_matrix_set_color_all(0, 0, 0);
    break;
  }
  return true;
}

// clang-format on
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // // Activate auto sentence case
    // if (!process_sentence_case(keycode, record)) {
    //     return false;
    // }

    // This will do most of the grunt work with the keycodes.
    switch (keycode) {
        case RGB_SLD:
            if (record->event.pressed) {
                rgblight_mode(1);
            }
            return false;
            // case KC_ESC:
            //     if (get_oneshot_mods() != 0) {
            //         clear_oneshot_mods();
            //     }
            //     break;
            // case SUPER_ALT_TAB:
            //     if (record->event.pressed) {
            //         if (!is_alt_tab_active) {
            //             is_alt_tab_active = true;
            //             register_code(KC_LEFT_CTRL);
            //         }
            //         // alt_tab_timer = timer_read();
            //         register_code(KC_TAB);
            //     } else {
            //         unregister_code(KC_TAB);
            //     }
            //     break;
            //     return false;
            // case COLON_EQUAL:
            //     if (record->event.pressed) {
            //         SEND_STRING(":=");
            //     }
            //     break;
    }
    return true;
}

void matrix_scan_user(void) {
    // if (is_alt_tab_active) {
    //     if (timer_elapsed(alt_tab_timer) > 750) {
    //         unregister_code(KC_LEFT_CTRL);
    //         is_alt_tab_active = false;
    //     }
    // }
}

void leader_start_user(void) {
    is_leader_key_pressed = true;

    ergodox_right_led_1_on();
    ergodox_right_led_2_on();
    ergodox_right_led_3_on();

    rgb_matrix_set_color_all(255, 255, 255); // White

    // One taps
    rgb_matrix_set_color(IDX_C, 255, 0, 0); // Red
    rgb_matrix_set_color(IDX_V, 255, 0, 0); // Red
    rgb_matrix_set_color(IDX_1, 255, 0, 0); // Red

    // Starters taps
    rgb_matrix_set_color(IDX_M, 0, 255, 0); // Blue
    rgb_matrix_set_color(IDX_N, 0, 255, 0); // Blue
    rgb_matrix_set_color(IDX_3, 0, 255, 0); // Blue
    rgb_matrix_set_color(IDX_4, 0, 255, 0); // Blue

    // Additional taps
    rgb_matrix_set_color(IDX_T, 0, 0, 255); // Blue
    rgb_matrix_set_color(IDX_P, 0, 0, 255); // Blue
    rgb_matrix_set_color(IDX_W, 0, 0, 255); // Blue
    rgb_matrix_set_color(IDX_U, 0, 0, 255); // Blue
}

// Runs just one time when the keyboard initializes.
void keyboard_post_init_user(void) {
    rgb_matrix_enable();
    // ergodox_led_all_set(255);
    // rgblight_setrgb(0x00, 0x00, 0xFF);
    // rgb_matrix_set_color_all(97, 0, 255); // Light up all the keys in purple
};

// uint8_t layer_state_set_user(uint8_t state) {
//     uint8_t layer = biton(state);
//     ergodox_board_led_off();
//     ergodox_right_led_1_off();
//     ergodox_right_led_2_off();
//     ergodox_right_led_3_off();
//     switch (layer) {
//         case 1:
//             ergodox_right_led_1_on();
//             break;
//         case 2:
//             ergodox_right_led_2_on();
//             break;
//         case 3:
//             ergodox_right_led_3_on();
//             break;
//         case 4:
//             ergodox_right_led_1_on();
//             ergodox_right_led_2_on();
//             break;
//         case 5:
//             ergodox_right_led_1_on();
//             ergodox_right_led_3_on();
//             break;
//         case 6:
//             ergodox_right_led_2_on();
//             ergodox_right_led_3_on();
//             break;
//         case 7:
//             ergodox_right_led_1_on();
//             ergodox_right_led_2_on();
//             ergodox_right_led_3_on();
//             break;
//         default:
//             break;
//     }
//     return state;
// };

// Runs whenever there is a layer state change.
layer_state_t layer_state_set_user(layer_state_t state) {
    // if (is_alt_tab_active) {
    //     unregister_code(KC_LEFT_CTRL);
    //     is_alt_tab_active = false;
    // }

    // // Don't change layer color back to base if the leader key was pressed
    // if (is_leader_key_pressed) return state;

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    uint8_t layer = get_highest_layer(state);
    switch (layer) {
        case BASE:
            // rgb_matrix_set_color_all(97, 0, 255); // Purple
            break;
        case SHORTCUTS:
            // rgb_matrix_set_color_all(255, 149, 0); // Orange
            ergodox_right_led_1_on();
            break;
        case SYMBOLS:
            // rgb_matrix_set_color_all(255, 149, 0); // Orange
            ergodox_right_led_1_on();
            break;
        case MOUSE:
            // rgb_matrix_set_color_all(23, 200, 34); // Green

            // Orange for the keys that are actually changed
            // rgb_matrix_set_color(IDX_1, 255, 149, 0);
            // rgb_matrix_set_color(IDX_2, 255, 149, 0);
            // rgb_matrix_set_color(IDX_W, 255, 149, 0);
            // rgb_matrix_set_color(IDX_E, 255, 149, 0);
            // rgb_matrix_set_color(IDX_R, 255, 149, 0);
            // rgb_matrix_set_color(IDX_A, 255, 149, 0);
            // rgb_matrix_set_color(IDX_S, 255, 149, 0);
            // rgb_matrix_set_color(IDX_D, 255, 149, 0);
            // rgb_matrix_set_color(IDX_F, 255, 149, 0);
            // rgb_matrix_set_color(IDX_G, 255, 149, 0);
            // rgb_matrix_set_color(IDX_L4, 255, 149, 0);

            // rgb_matrix_set_color(IDX_U, 255, 149, 0);
            // rgb_matrix_set_color(IDX_I, 255, 149, 0);
            // rgb_matrix_set_color(IDX_O, 255, 149, 0);
            // rgb_matrix_set_color(IDX_H, 255, 149, 0);
            // rgb_matrix_set_color(IDX_J, 255, 149, 0);
            // rgb_matrix_set_color(IDX_K, 255, 149, 0);
            // rgb_matrix_set_color(IDX_L, 255, 149, 0);
            // rgb_matrix_set_color(IDX_Colon, 255, 149, 0);

            ergodox_right_led_2_on();
            break;
        case NUMBERS:
            // rgb_matrix_set_color_all(0, 0, 255); // Blue

            // Orange for the number keys
            // rgb_matrix_set_color(IDX_U, 255, 149, 0);
            // rgb_matrix_set_color(IDX_I, 255, 149, 0);
            // rgb_matrix_set_color(IDX_O, 255, 149, 0);
            // rgb_matrix_set_color(IDX_P, 23, 200, 34);
            // rgb_matrix_set_color(IDX_J, 255, 149, 0);
            // rgb_matrix_set_color(IDX_K, 255, 149, 0);
            // rgb_matrix_set_color(IDX_L, 255, 149, 0);
            // rgb_matrix_set_color(IDX_Colon, 23, 200, 34);
            // rgb_matrix_set_color(IDX_M, 255, 149, 0);
            // rgb_matrix_set_color(IDX_Period, 255, 149, 0);
            // rgb_matrix_set_color(IDX_Comma, 255, 149, 0);
            // rgb_matrix_set_color(IDX_F_Slash, 23, 200, 34);
            // rgb_matrix_set_color(IDX_R1, 255, 149, 0);
            // rgb_matrix_set_color(IDX_R2, 23, 200, 34);
            // rgb_matrix_set_color(IDX_R3, 23, 200, 34);
            // rgb_matrix_set_color(IDX_R4, 23, 200, 34);
            ergodox_right_led_3_on();
            break;
        case GAMING:
            // rgb_matrix_set_color_all(255, 255, 0);       // Red
            // rgb_matrix_set_color(IDX_L4, 255, 255, 255); // White
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            break;
        case MEDIA:
            // rgb_matrix_set_color_all(64, 224, 208); // Orange
            ergodox_right_led_1_on();
            ergodox_right_led_3_on();
            break;
        default:
            break;
    }

    return state;
};
