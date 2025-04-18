#include QMK_KEYBOARD_H

#include "./key_indexes.h"
#include "../../../users/campasachamp/campasachamp.h"
#include "../../../users/campasachamp/features/sentence_case.h"

enum layers {
    BASE,      // default layer
    SYMBOLS,
    MEDIA,
    NUMBERS,
    MOUSE,
    GAMING,
};


// State Variables
static bool is_leader_key_pressed = false;
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

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
  TD(TD_CAPS) ,         MY_MEH_Z,         MY_HYPER_X,          KC_C,        KC_V,           KC_B,   LCTL(KC_SPACE),      TD(TD_CAPS)         , KC_N       , KC_M        , KC_COMMA    , KC_DOT      , MT(MOD_RCTL, KC_SLASH)            , TD(TD_CAPS)     ,
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
