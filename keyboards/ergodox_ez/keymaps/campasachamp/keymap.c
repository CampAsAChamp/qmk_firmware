#include QMK_KEYBOARD_H

#include "features/sentence_case.h"
#include "../../../../shared/secrets.h"


// RGB Matrix Indexes
static const int IDX_1 = 28;
static const int IDX_2 = 27;
static const int IDX_3 = 26;
static const int IDX_4 = 25;
static const int IDX_5 = 24;
static const int IDX_Q = 33;
static const int IDX_W = 32;
static const int IDX_E = 31;
static const int IDX_R = 30;
static const int IDX_T = 29;
static const int IDX_A = 38;
static const int IDX_S = 37;
static const int IDX_D = 36;
static const int IDX_F = 35;
static const int IDX_G = 34;
static const int IDX_Z = 43;
static const int IDX_X = 42;
static const int IDX_C = 41;
static const int IDX_V = 40;
static const int IDX_B = 39;
static const int IDX_L1 = 47;
static const int IDX_L2 = 46;
static const int IDX_L3 = 45;
static const int IDX_L4 = 44;
static const int IDX_6 = 0;
static const int IDX_7 = 1;
static const int IDX_8 = 2;
static const int IDX_9 = 3;
static const int IDX_0 = 4;
static const int IDX_Y = 5;
static const int IDX_U = 6;
static const int IDX_I = 7;
static const int IDX_O = 8;
static const int IDX_P = 9;
static const int IDX_H = 10;
static const int IDX_J = 11;
static const int IDX_K = 12;
static const int IDX_L = 13;
static const int IDX_Colon = 14;
static const int IDX_N = 15;
static const int IDX_M = 16;
static const int IDX_Comma = 17;
static const int IDX_Period = 18;
static const int IDX_F_Slash = 19;
static const int IDX_R1 = 20;
static const int IDX_R2 = 21;
static const int IDX_R3 = 22;
static const int IDX_R4 = 23;

#define MY_HYPER S(G(C(KC_LALT)))
#define MY_MEH S(G(KC_LALT))

#define MY_HYPER_X MT(MOD_LSFT | MOD_LGUI | MOD_LCTL | MOD_LALT, KC_X)
#define MY_MEH_Z MT(MOD_LSFT | MOD_LGUI | MOD_LALT, KC_Z)

enum custom_keycodes {
    SUPER_ALT_TAB = SAFE_RANGE,
    COLON_EQUAL,
};

enum layers {
    BASE,      // default layer
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

// State Variables
static bool is_leader_key_pressed = false;
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

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

// Tap Dance definitions
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
  TD(TD_CAPS) ,         MY_MEH_Z,         MY_HYPER_X,         LT(MEDIA, KC_C),         KC_V,           KC_B,   LCTL(KC_SPACE),      TD(TD_CAPS)         , KC_N       , KC_M        , KC_COMMA    , KC_DOT      , MT(MOD_RCTL, KC_SLASH)            , TD(TD_CAPS)     ,
  _______,       _______,      KC_LPRN, KC_RPRN,  MO(SYMBOLS),                                                                           TT(MOUSE)   , KC_LCBR     , KC_RCBR     , KC_LBRC             , KC_RBRC    ,

                                                                   LALT(KC_SPACE), LCTL(LSFT(KC_1)),      TG(GAMING)          , TT(NUMBERS),
                                                                                   LCTL(LSFT(KC_2)),      _______             ,
                                                         KC_SPACE,      KC_DELETE, LCTL(LSFT(KC_5)),      LCMD(LCTL(KC_SPACE)), KC_ENTER   , KC_BSPC
),

[SYMBOLS] = LAYOUT_ergodox_pretty(
     LCTL(KC_GRV),      KC_F1,      KC_F2,      KC_F3,      KC_F4,   KC_F5,  KC_F11,      KC_F12 , KC_F6         , KC_F7    , KC_F8  , KC_F9   , KC_F10        , TO(BASE),
     SUPER_ALT_TAB, LCTL(KC_Q), LCTL(KC_W),    _______,    _______, KC_LBRC, _______,      _______, KC_RBRC       , KC_HOME  , KC_UP  , KC_END  , _______       , _______ ,
     LCTL(KC_GRV),    _______,    _______,    _______,    _______, KC_LCBR,                        KC_RCBR       , KC_LEFT  , KC_DOWN, KC_RIGHT, _______       , KC_GRAVE,
     _______, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), _______, KC_CAPS,      TD(TD_CAPS), KC_MINS       , KC_UNDS  , _______, _______ , _______, _______ ,
     _______,    _______,    _______,    _______,    _______,                                                 _______  , _______, _______ , _______       , _______ ,

                                                              _______, _______,      _______, _______       ,
                                                                       _______,      _______,
                                                     _______, _______, QK_LEAD,      _______, LCTL(KC_ENTER), LALT(KC_BSPC)
),

[MEDIA] = LAYOUT_ergodox_pretty(
LCTL(KC_GRV),      KC_F1,      KC_F2,      KC_F3,      KC_F4,   KC_F5,  KC_F11,      KC_F12 , KC_F6         , KC_F7    , KC_F8  , KC_F9   , KC_F10        , TO(BASE),
_______, KC_MPRV, KC_MPLY,  KC_MNXT, _______, _______,            _______,      _______, _______, _______, _______, _______, _______, _______ ,
_______, KC_VOLD, _______,  KC_VOLU, _______, _______,                                   _______, _______, _______, _______, _______, _______ ,
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

// clang-format on
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    // Activate auto sentence case
    if (!process_sentence_case(keycode, record)) { 
        return false; 
    }

    // This will do most of the grunt work with the keycodes.
    switch (keycode) { 
        case KC_ESC:
            if (get_oneshot_mods() != 0){
                clear_oneshot_mods();
            }
            break;
        case SUPER_ALT_TAB:
            if (record->event.pressed){
                if (!is_alt_tab_active){
                    is_alt_tab_active = true;
                    register_code(KC_LEFT_CTRL);
                }
                // alt_tab_timer = timer_read();
                register_code(KC_TAB);
            } 
            else {
                unregister_code(KC_TAB);
            }
            break;
            return false;
        case COLON_EQUAL:
            if (record->event.pressed) 
                SEND_STRING(":=");
                
            break;
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

    rgb_matrix_set_color_all(255, 255, 255);  // White

    // One taps
    rgb_matrix_set_color(IDX_C, 255, 0, 0);   // Red
    rgb_matrix_set_color(IDX_V, 255, 0, 0);   // Red
    rgb_matrix_set_color(IDX_1, 255, 0, 0);   // Red

    // Starters taps
    rgb_matrix_set_color(IDX_M, 0, 255, 0);   // Blue
    rgb_matrix_set_color(IDX_N, 0, 255, 0);   // Blue
    rgb_matrix_set_color(IDX_3, 0, 255, 0);   // Blue
    rgb_matrix_set_color(IDX_4, 0, 255, 0);   // Blue

    // Additional taps
    rgb_matrix_set_color(IDX_T, 0, 0, 255);   // Blue
    rgb_matrix_set_color(IDX_P, 0, 0, 255);   // Blue
    rgb_matrix_set_color(IDX_W, 0, 0, 255);   // Blue
    rgb_matrix_set_color(IDX_U, 0, 0, 255);   // Blue
}

void leader_end_user(void) {
    if (leader_sequence_one_key(KC_C)) {
        send_string(USERNAME);
    }
    else if (leader_sequence_two_keys(KC_N, KC_P)) {
        send_string(P_EMAIL);
    } 
    else if (leader_sequence_two_keys(KC_N, KC_W)) {
        send_string(W_EMAIL);
    } 
    else if (leader_sequence_two_keys(KC_N, KC_U)) {
        send_string(S_EMAIL);
    }
    else if (leader_sequence_one_key(KC_V)) {
        send_string(V);
    }
    else if (leader_sequence_one_key(KC_1)) {
        send_string(MEGA);
    }
    else if (leader_sequence_one_key(KC_4)) {
        send_string(FOUR);
    }
    else if (leader_sequence_two_keys(KC_4, KC_T)) {
        send_string(FOUR_CAPS);
    }
    else if (leader_sequence_two_keys(KC_4, KC_4)) {
        send_string(FOUR_SYM);
    }
    else if (leader_sequence_one_key(KC_3)) {
        send_string(PHONE_NUM);
    }
    else if (leader_sequence_two_keys(KC_3, KC_TAB)) {
        send_string(PHONE_NUM_TAB);
    }
    else if (leader_sequence_two_keys(KC_M, KC_P)) {
        send_string(M);
    }
    else if (leader_sequence_three_keys(KC_M, KC_P, KC_P)) {
        send_string(M_SYM);
    }

    // Set RGB back to the base layers
    is_leader_key_pressed = false;
    rgb_matrix_set_color_all(97, 0, 255); // Purple
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
}

// Runs just one time when the keyboard initializes.
void keyboard_post_init_user(void) {
    ergodox_led_all_set(255);
    rgb_matrix_set_color_all(97, 0, 255); //Light up all the keys in purple
};

// Runs whenever there is a layer state change.
layer_state_t layer_state_set_user(layer_state_t state) {
    if (is_alt_tab_active) {
        unregister_code(KC_LEFT_CTRL);
        is_alt_tab_active = false;
    }
   
    // Don't change layer color back to base if the leader key was pressed
    if (is_leader_key_pressed)
        return state;

   

    // ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    uint8_t layer = get_highest_layer(state);
    switch (layer) {
        case BASE:
            rgb_matrix_set_color_all(97, 0, 255); // Purple
            break;
        case SYMBOLS:
            rgb_matrix_set_color_all(255, 149, 0); // Orange
            ergodox_right_led_1_on();
            break;
        case MOUSE:
            rgb_matrix_set_color_all(23, 200, 34); // Green

            // Orange for the keys that are actually changed
            rgb_matrix_set_color(IDX_1, 255, 149, 0);
            rgb_matrix_set_color(IDX_2, 255, 149, 0);
            rgb_matrix_set_color(IDX_W, 255, 149, 0);
            rgb_matrix_set_color(IDX_E, 255, 149, 0);
            rgb_matrix_set_color(IDX_R, 255, 149, 0);
            rgb_matrix_set_color(IDX_A, 255, 149, 0);
            rgb_matrix_set_color(IDX_S, 255, 149, 0);
            rgb_matrix_set_color(IDX_D, 255, 149, 0);
            rgb_matrix_set_color(IDX_F, 255, 149, 0);
            rgb_matrix_set_color(IDX_G, 255, 149, 0);
            rgb_matrix_set_color(IDX_L4, 255, 149, 0);

            rgb_matrix_set_color(IDX_U, 255, 149, 0);
            rgb_matrix_set_color(IDX_I, 255, 149, 0);
            rgb_matrix_set_color(IDX_O, 255, 149, 0);
            rgb_matrix_set_color(IDX_H, 255, 149, 0);
            rgb_matrix_set_color(IDX_J, 255, 149, 0);
            rgb_matrix_set_color(IDX_K, 255, 149, 0);
            rgb_matrix_set_color(IDX_L, 255, 149, 0);
            rgb_matrix_set_color(IDX_Colon, 255, 149, 0);
            
            ergodox_right_led_2_on();
            break;
        case NUMBERS:
            rgb_matrix_set_color_all(0, 0, 255); // Blue

            // Orange for the number keys
            rgb_matrix_set_color(IDX_U, 255, 149, 0);
            rgb_matrix_set_color(IDX_I, 255, 149, 0);
            rgb_matrix_set_color(IDX_O, 255, 149, 0);
            rgb_matrix_set_color(IDX_P, 23, 200, 34);
            rgb_matrix_set_color(IDX_J, 255, 149, 0);
            rgb_matrix_set_color(IDX_K, 255, 149, 0);
            rgb_matrix_set_color(IDX_L, 255, 149, 0);
            rgb_matrix_set_color(IDX_Colon, 23, 200, 34);
            rgb_matrix_set_color(IDX_M, 255, 149, 0);
            rgb_matrix_set_color(IDX_Period, 255, 149, 0);
            rgb_matrix_set_color(IDX_Comma, 255, 149, 0);
            rgb_matrix_set_color(IDX_F_Slash, 23, 200, 34);
            rgb_matrix_set_color(IDX_R1, 255, 149, 0);
            rgb_matrix_set_color(IDX_R2, 23, 200, 34);
            rgb_matrix_set_color(IDX_R3, 23, 200, 34);
            rgb_matrix_set_color(IDX_R4, 23, 200, 34);
            ergodox_right_led_3_on();
            break;
        case GAMING:
            rgb_matrix_set_color_all(255, 0, 0); // Red
            rgb_matrix_set_color(IDX_L4, 255, 255, 255); // White
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            break;
        case MEDIA:
            rgb_matrix_set_color_all(64, 224, 208); // Orange
            ergodox_right_led_1_on();
            ergodox_right_led_3_on();
            break;
        default:
            break;
    }

    return state;
};
