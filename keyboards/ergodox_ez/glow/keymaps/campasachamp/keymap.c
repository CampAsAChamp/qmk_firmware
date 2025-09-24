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
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

enum td_keycodes {
    TD_DASH,
    TD_PLUS,
    TD_LBRACKET,
    TD_RBRACKET,
    TD_PIPE,
    TD_COPY,
    TD_UNDO,
    TD_CAPS_BASIC,
    TD_CAPS_ADVANCED,
    X_CTL,
};

enum custom_keycodes {
    SUPER_ALT_TAB = SAFE_RANGE,
    RGB_SLD,
    COLON_EQUAL,
};

// State detection helper
td_state_t cur_dance(tap_dance_state_t *state);

// For the x tap dance. Put it here so it can be used in any keymap
void dance_caps_finished(tap_dance_state_t *state, void *user_data);
void dance_caps_reset(tap_dance_state_t *state, void *user_data);

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
    [TD_CAPS_BASIC] = ACTION_TAP_DANCE_FN(dance_caps),
    [TD_CAPS_ADVANCED] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_caps_finished, dance_caps_reset)

};

const uint16_t PROGMEM HOMEROW_APP_ACTIVATION[] = {KC_G, KC_H, COMBO_END};
combo_t key_combos[] = {
    COMBO(HOMEROW_APP_ACTIVATION, HYPR(KC_ENTER)) // Pressing G + H together sends Hyper + Enter,
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
  TD(TD_CAPS_BASIC) ,         MY_MEH_Z,         MY_HYPER_X,          LT(SYMBOLS, KC_C),        KC_V,           KC_B,   LCTL(KC_SPACE),      _______         , KC_N       , KC_M        , LT(SYMBOLS, KC_COMMA)    , MY_HYPER_DOT      , MT(MOD_RCTL, KC_SLASH)            , TD(TD_CAPS_BASIC)     ,
  TOGGLE_LAYER_COLOR,       _______,      _______, _______,  MO(SHORTCUTS),                                                                           TT(MOUSE)   , _______     , _______     , _______             , _______    ,

                                                                   LALT(KC_SPACE), LCTL(LSFT(KC_1)),      TG(GAMING)          , TT(NUMBERS),
                                                                                   LCTL(LSFT(KC_2)),      _______             ,
                                                         KC_SPACE,      KC_DELETE, LCTL(LSFT(KC_5)),      LCMD(LCTL(KC_SPACE)), KC_ENTER   , KC_BSPC
),

[SHORTCUTS] = LAYOUT_ergodox_pretty(
     LCTL(KC_GRV),      KC_F1,      KC_F2,      KC_F3,      KC_F4,   KC_F5,  KC_F11,      KC_F12 , KC_F6         , KC_F7    , KC_F8  , KC_F9   , KC_F10        , TO(BASE),
     SUPER_ALT_TAB, LCTL(KC_Q), LCTL(KC_W),    _______,    _______, KC_LBRC, _______,      _______, KC_RBRC       , KC_HOME  , KC_UP  , KC_END  , _______       , _______ ,
     LCTL(KC_GRV),    _______,    _______,    _______,    _______, KC_LCBR,                        KC_RCBR       , KC_LEFT  , KC_DOWN, KC_RIGHT, _______       , KC_GRAVE,
     _______, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), _______, KC_CAPS,      TD(TD_CAPS_BASIC), KC_MINS       , KC_UNDS  , _______, _______ , _______, _______ ,
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
_______, KC_MPRV, KC_MPLY,  KC_MNXT, _______, _______,            _______,      _______, _______, _______, _______, _______, _______, _______ ,
_______, _______, KC_VOLD,  KC_VOLU, _______, _______,                                   _______, _______, _______, _______, _______, _______ ,
_______, _______, _______, _______,  _______, _______,            _______,      _______, _______, _______, _______, _______, _______, _______ ,
_______, _______, _______, _______, _______,                                                     _______, _______, _______, _______, _______ ,

                                              _______,     RGB_MODE_PLAIN,      _______, _______,
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
_______, MS_ACL0, MS_ACL1, MS_ACL2, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______,
_______, _______, MS_WHLU, MS_UP, MS_WHLD, _______, _______,      _______, _______, _______, _______, _______, _______, QK_BOOT,
_______, MS_WHLL, MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLL,                        _______, _______, _______, _______, _______, _______,
MS_BTN4, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, MS_BTN5,
_______, _______, _______, _______, MS_BTN1,                                          _______, _______, _______, _______, _______,

                                             _______, _______,      _______, _______,
                                                      _______,      _______,
                                    MS_BTN2, MS_BTN3, _______,      _______, _______, _______
),

[GAMING] = LAYOUT_ergodox_pretty(
KC_TILDE, _______, _______, _______,  _______, _______,     _______,      _______, _______, _______, _______, _______, _______     , TO(BASE),
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

// Old ledmap array removed - now using enum-based approach with set_layer_keys()

// Helper function to set LED color for a specific key using key index enum
void set_key_color(int key_index, uint8_t h, uint8_t s, uint8_t v) {
    HSV hsv = { .h = h, .s = s, .v = v };
    if (!hsv.h && !hsv.s && !hsv.v) {
        rgb_matrix_set_color(key_index, 0, 0, 0);
    } else {
        RGB rgb = hsv_to_rgb(hsv);
        float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
        rgb_matrix_set_color(key_index, f * rgb.r, f * rgb.g, f * rgb.b);
    }
}

// Helper function to set all LEDs to a base color
void set_all_keys_color(uint8_t h, uint8_t s, uint8_t v) {
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        set_key_color(i, h, s, v);
    }
}

// Helper function to set specific keys for each layer
void set_layer_keys(int layer) {
    switch (layer) {
        case BASE: // Layer 0 - All keys lit
            set_all_keys_color(188, 255, 255);
            break;

        case SHORTCUTS: // Layer 1 - Function keys and shortcuts
            set_all_keys_color(0, 245, 245); // Dim all first
            // Function keys (F1-F12)
            set_key_color(IDX_1, 41, 255, 255);
            set_key_color(IDX_2, 41, 255, 255);
            set_key_color(IDX_3, 41, 255, 255);
            set_key_color(IDX_4, 41, 255, 255);
            set_key_color(IDX_5, 41, 255, 255);
            set_key_color(IDX_6, 41, 255, 255);
            set_key_color(IDX_7, 41, 255, 255);
            set_key_color(IDX_8, 41, 255, 255);
            set_key_color(IDX_9, 41, 255, 255);
            set_key_color(IDX_0, 41, 255, 255);
            // Shortcut keys
            set_key_color(IDX_Q, 131, 255, 255);
            set_key_color(IDX_W, 131, 255, 255);
            set_key_color(IDX_E, 131, 255, 255);
            set_key_color(IDX_R, 131, 255, 255);
            set_key_color(IDX_T, 131, 255, 255);
            set_key_color(IDX_Y, 131, 255, 255);
            set_key_color(IDX_U, 131, 255, 255);
            set_key_color(IDX_I, 131, 255, 255);
            set_key_color(IDX_O, 131, 255, 255);
            set_key_color(IDX_P, 131, 255, 255);
            set_key_color(IDX_A, 131, 255, 255);
            set_key_color(IDX_S, 131, 255, 255);
            set_key_color(IDX_D, 131, 255, 255);
            set_key_color(IDX_F, 131, 255, 255);
            set_key_color(IDX_G, 131, 255, 255);
            set_key_color(IDX_H, 131, 255, 255);
            set_key_color(IDX_J, 131, 255, 255);
            set_key_color(IDX_K, 131, 255, 255);
            set_key_color(IDX_L, 131, 255, 255);
            set_key_color(IDX_Colon, 131, 255, 255);
            set_key_color(IDX_Z, 131, 255, 255);
            set_key_color(IDX_X, 131, 255, 255);
            set_key_color(IDX_C, 131, 255, 255);
            set_key_color(IDX_V, 131, 255, 255);
            set_key_color(IDX_B, 131, 255, 255);
            set_key_color(IDX_N, 131, 255, 255);
            set_key_color(IDX_M, 131, 255, 255);
            set_key_color(IDX_Comma, 131, 255, 255);
            set_key_color(IDX_Period, 131, 255, 255);
            set_key_color(IDX_F_Slash, 131, 255, 255);
            break;

        case SYMBOLS: // Layer 2 - Only bracket/parenthesis keys
            set_all_keys_color(0, 245, 245); // Dim all first
            // Symbol keys (brackets, parentheses)
            set_key_color(IDX_Q, 0, 245, 245);
            set_key_color(IDX_W, 0, 245, 245);
            set_key_color(IDX_E, 0, 245, 245);
            set_key_color(IDX_R, 0, 245, 245);
            set_key_color(IDX_T, 0, 245, 245);
            set_key_color(IDX_Y, 0, 245, 245);
            set_key_color(IDX_U, 0, 245, 245);
            set_key_color(IDX_I, 0, 245, 245);
            set_key_color(IDX_O, 0, 245, 245);
            set_key_color(IDX_P, 0, 245, 245);
            set_key_color(IDX_A, 0, 245, 245);
            set_key_color(IDX_S, 0, 245, 245);
            set_key_color(IDX_D, 0, 245, 245);
            set_key_color(IDX_F, 0, 245, 245);
            set_key_color(IDX_G, 0, 245, 245);
            set_key_color(IDX_H, 0, 245, 245);
            set_key_color(IDX_J, 0, 245, 245);
            set_key_color(IDX_K, 0, 245, 245);
            set_key_color(IDX_L, 0, 245, 245);
            set_key_color(IDX_Colon, 0, 245, 245);
            set_key_color(IDX_Z, 0, 245, 245);
            set_key_color(IDX_X, 0, 245, 245);
            set_key_color(IDX_C, 0, 245, 245);
            set_key_color(IDX_V, 0, 245, 245);
            set_key_color(IDX_B, 0, 245, 245);
            set_key_color(IDX_N, 0, 245, 245);
            set_key_color(IDX_M, 0, 245, 245);
            set_key_color(IDX_Comma, 0, 245, 245);
            set_key_color(IDX_Period, 0, 245, 245);
            set_key_color(IDX_F_Slash, 0, 245, 245);
            break;

        case MEDIA: // Layer 3 - Media controls and function keys
            set_all_keys_color(0, 245, 245); // Dim all first
            // Function keys (F1-F12)
            set_key_color(IDX_1, 41, 255, 255);
            set_key_color(IDX_2, 41, 255, 255);
            set_key_color(IDX_3, 41, 255, 255);
            set_key_color(IDX_4, 41, 255, 255);
            set_key_color(IDX_5, 41, 255, 255);
            set_key_color(IDX_6, 41, 255, 255);
            set_key_color(IDX_7, 41, 255, 255);
            set_key_color(IDX_8, 41, 255, 255);
            set_key_color(IDX_9, 41, 255, 255);
            set_key_color(IDX_0, 41, 255, 255);
            // Media control keys
            set_key_color(IDX_Q, 131, 255, 255);
            set_key_color(IDX_W, 131, 255, 255);
            set_key_color(IDX_E, 131, 255, 255);
            set_key_color(IDX_R, 131, 255, 255);
            set_key_color(IDX_T, 131, 255, 255);
            set_key_color(IDX_Y, 131, 255, 255);
            set_key_color(IDX_U, 131, 255, 255);
            set_key_color(IDX_I, 131, 255, 255);
            set_key_color(IDX_O, 131, 255, 255);
            set_key_color(IDX_P, 131, 255, 255);
            set_key_color(IDX_A, 131, 255, 255);
            set_key_color(IDX_S, 131, 255, 255);
            set_key_color(IDX_D, 131, 255, 255);
            set_key_color(IDX_F, 131, 255, 255);
            set_key_color(IDX_G, 131, 255, 255);
            set_key_color(IDX_H, 131, 255, 255);
            set_key_color(IDX_J, 131, 255, 255);
            set_key_color(IDX_K, 131, 255, 255);
            set_key_color(IDX_L, 131, 255, 255);
            set_key_color(IDX_Colon, 131, 255, 255);
            set_key_color(IDX_Z, 131, 255, 255);
            set_key_color(IDX_X, 131, 255, 255);
            set_key_color(IDX_C, 131, 255, 255);
            set_key_color(IDX_V, 131, 255, 255);
            set_key_color(IDX_B, 131, 255, 255);
            set_key_color(IDX_N, 131, 255, 255);
            set_key_color(IDX_M, 131, 255, 255);
            set_key_color(IDX_Comma, 131, 255, 255);
            set_key_color(IDX_Period, 131, 255, 255);
            set_key_color(IDX_F_Slash, 131, 255, 255);
            break;

        case NUMBERS: // Layer 4 - Number pad and navigation
            set_all_keys_color(0, 245, 245); // Dim all first
            // Number keys
            set_key_color(IDX_U, 0, 0, 255);
            set_key_color(IDX_I, 0, 0, 255);
            set_key_color(IDX_O, 0, 0, 255);
            set_key_color(IDX_J, 0, 0, 255);
            set_key_color(IDX_K, 0, 0, 255);
            set_key_color(IDX_L, 0, 0, 255);
            set_key_color(IDX_M, 0, 0, 255);
            set_key_color(IDX_Comma, 0, 0, 255);
            set_key_color(IDX_Period, 0, 0, 255);
            set_key_color(IDX_F_Slash, 0, 0, 255);
            // Special keys
            set_key_color(IDX_P, 0, 0, 255);
            set_key_color(IDX_Colon, 0, 0, 255);
            set_key_color(IDX_R1, 0, 0, 255);
            set_key_color(IDX_R2, 0, 0, 255);
            set_key_color(IDX_R3, 0, 0, 255);
            set_key_color(IDX_R4, 0, 0, 255);
            break;

        case MOUSE: // Layer 5 - Mouse controls
            set_all_keys_color(34, 215, 193);
            break;

        case GAMING: // Layer 6 - Gaming layout
            set_all_keys_color(0, 245, 245); // Dim all first
            // Gaming keys
            set_key_color(IDX_1, 255, 255, 0);
            set_key_color(IDX_A, 255, 255, 0);
            set_key_color(IDX_S, 255, 255, 0);
            set_key_color(IDX_D, 255, 255, 0);
            set_key_color(IDX_F, 255, 255, 0);
            set_key_color(IDX_J, 255, 255, 0);
            set_key_color(IDX_K, 255, 255, 0);
            set_key_color(IDX_L, 255, 255, 0);
            set_key_color(IDX_Colon, 255, 255, 0);
            break;

        default:
            break;
    }
}

// Old set_layer_color function removed - now using enum-based set_layer_keys()

bool rgb_matrix_indicators_user(void) {
  if (keyboard_config.disable_layer_led) { return false; }
  switch (biton32(layer_state)) {
    case BASE:
      set_layer_keys(BASE);
      break;
    case SHORTCUTS:
      set_layer_keys(SHORTCUTS);
      break;
    case SYMBOLS:
      set_layer_keys(SYMBOLS);
      break;
    case MEDIA:
      set_layer_keys(MEDIA);
      break;
    case NUMBERS:
      set_layer_keys(NUMBERS);
      break;
    case MOUSE:
      set_layer_keys(MOUSE);
      break;
    case GAMING:
      set_layer_keys(GAMING);
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

/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicative that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 *
 * One thing that is currently not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
 *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
 *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
 *
 * Good places to put an advanced tap dance:
 *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
 *
 * Criteria for "good placement" of a tap dance key:
 *  Not a key that is hit frequently in a sentence
 *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
 *    in a web form. So 'tab' would be a poor choice for a tap dance.
 *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
 *    letter 'p', the word 'pepper' would be quite frustrating to type.
 *
 * For the third point, there does exist the 'TD_DOUBLE_SINGLE_TAP', however this is not fully tested
 *
 */
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

// Create an instance of 'td_tap_t' for the 'x' tap dance.
static td_tap_t xtap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void dance_caps_finished(tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case TD_SINGLE_TAP:  set_oneshot_mods(MOD_BIT(KC_LSFT)); break;
        case TD_SINGLE_HOLD: register_code(KC_LSFT); break;
        case TD_DOUBLE_TAP:  caps_word_on(); break;
        case TD_TRIPLE_TAP:  tap_code(KC_CAPS); break;
        default: break;
    }
}

void dance_caps_reset(tap_dance_state_t *state, void *user_data) {
    switch (xtap_state.state) {
        case TD_SINGLE_HOLD: unregister_code(KC_LSFT); break;
        default: break;
    }
    xtap_state.state = TD_NONE;
}
