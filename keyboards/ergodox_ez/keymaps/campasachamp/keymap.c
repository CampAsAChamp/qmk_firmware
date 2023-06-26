#include QMK_KEYBOARD_H
#include "version.h"
#include "secrets.h"

// State Variables
static bool is_leader_key_pressed = false;
bool is_cmd_tab_active = false;
uint16_t cmd_tab_timer = 0;

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

enum custom_keycodes {
    VRSN = SAFE_RANGE,
    CMD_TAB,
    COLON_EQUAL,
};

enum layers {
    BASE,      // default layer
    SYMBOLS,
    MOUSE,
    NUMBERS,
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
};

void dance_l_bracket (tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        // Left Parenthese -- (
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
        // Right Parenthese -- )
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
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   =    |   1  |   2  |   3  |   4  |   5  | LEFT |           | RIGHT|   6  |   7  |   8  |   9  |   0  |   -    |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Del    |   Q  |   W  |   E  |   R  |   T  |  L1  |           |  L1  |   Y  |   U  |   I  |   O  |   P  |   \    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | BkSp   |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |; / L2|' / Cmd |
 * |--------+------+------+------+------+------| Hyper|           | Meh  |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  | Ctrl| RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |Grv/L1|  '"  |AltShf| Left | Right|                                       |  Up  | Down |   [  |   ]  | ~L1  |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | App  | LGui |       | Alt  |Ctrl/Esc|
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      |      | Home |       | PgUp |        |      |
 *                                 | Space|Backsp|------|       |------|  Tab   |Enter |
 *                                 |      |ace   | End  |       | PgDn |        |      |
 *                                 `--------------------'       `----------------------'
 */
[BASE] = LAYOUT_ergodox_pretty(
    KC_ESCAPE,      KC_1,     KC_2,     KC_3,           KC_4,           KC_5,     KC_MINUS,              KC_EQUAL,         KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           KC_BSPC,
    KC_TAB,         KC_Q,     KC_W,     KC_E,           KC_R,           KC_T,     TD(TD_LBRACKET),       TD(TD_RBRACKET),  KC_Y,           KC_U,           KC_I,           KC_O,           KC_P,           TD(TD_PIPE),
    MO(SYMBOLS),    KC_A,     KC_S,     KC_D,           KC_F,           KC_G,                                              KC_H,           KC_J,           KC_K,           KC_L,           KC_SEMICOLON,   KC_QUOTE,
    KC_LSFT,        KC_Z,     KC_X,     KC_C,           KC_V,           KC_B,     KC_GRAVE,              CW_TOGG,          KC_N,           KC_M,           KC_COMMA,       KC_DOT,         KC_SLASH,       KC_RSFT,
    OSM(MOD_LCTL),  _______,  _______,  OSM(MOD_LALT),  OSM(MOD_LGUI),                                                                     TT(MOUSE),      TD(TD_UNDO),    TD(TD_COPY),    LGUI(KC_V),     CMD_TAB,
                                                                   LGUI(KC_SPACE), LALT(KC_SPACE),      TG(GAMING), TT(NUMBERS),          
                                                                                 LGUI(LSFT(KC_2)),       _______,
                                                              KC_SPACE, KC_DELETE, LGUI(KC_GRAVE),       _______, KC_ENTER, KC_BSPC

 ),
/* Keymap 1: Symbol Layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |Version  |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |      |  F6  |  F7  |  F8  |  F9  |  F10 |   F11  |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |   !  |   @  |   {  |   }  |   |  |      |           |      |   Up |   7  |   8  |   9  |   *  |   F12  |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |   #  |   $  |   (  |   )  |   `  |------|           |------| Down |   4  |   5  |   6  |   +  |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |   %  |   ^  |   [  |   ]  |   ~  |      |           |      |   &  |   1  |   2  |   3  |   \  |        |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | EPRM  |      |      |      |      |                                       |      |    . |   0  |   =  |      |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |Animat|      |       |Toggle|Solid |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |Bright|Bright|      |       |      |Hue-  |Hue+  |
 *                                 |ness- |ness+ |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[SYMBOLS] = LAYOUT_ergodox_pretty(
  VRSN,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F11,      KC_F12, KC_F6,   KC_F7,   KC_F8,   KC_F9,     KC_F10,  TO(BASE),
  _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, _______, _______,     _______, _______, KC_HOME, KC_UP,   KC_END,   _______, _______,
  _______, KC_VOLD, KC_VOLU, _______, _______, _______,                       _______, KC_LEFT, KC_DOWN, KC_RIGHT, COLON_EQUAL, _______,
  _______, _______, _______, _______, _______, _______, _______,     KC_CAPS, KC_MINS, KC_UNDS, _______, _______,  _______, _______,
  _______, _______, _______, _______, KC_LALT,                                         _______, _______, _______,  _______, _______,

                                               RGB_TOG, RGB_MODE_PLAIN,     _______, _______,
                                                        TOGGLE_LAYER_COLOR,     _______,
                                      _______, QK_LEAD, _______,      _______, KC_LCTL, KC_DELETE
),
/* Keymap 2: Mouse layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |------|           |------|      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[MOUSE] = LAYOUT_ergodox_pretty(
  _______, KC_ACL0, KC_ACL1, KC_ACL2, _______, _______, _______,     _______, _______, _______, _______, _______, _______, QK_BOOT,
  _______, _______, KC_WH_U, KC_MS_U, KC_WH_D, _______, _______,     _______, _______, KC_WH_U, KC_MS_U, KC_WH_D, _______, _______,
  _______, KC_WH_L, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_R,                       KC_WH_L, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_R, _______,
  KC_BTN4, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, KC_BTN5,
  _______, _______, _______, _______, KC_BTN1,                                         _______, _______, _______, _______, _______,

                                               _______, _______,     _______, _______,
                                                        _______,     _______,
                                      KC_BTN2, KC_BTN3, _______,     _______, _______, _______
),
/* Keymap 3: Number Pad
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |------|           |------|      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[NUMBERS] = LAYOUT_ergodox_pretty(
  _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, TO(BASE),
  _______, _______, _______, KC_MS_U, _______, _______, _______,     _______, _______, KC_7,    KC_8,    KC_9,    KC_ASTR, _______,
  _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______,                       _______, KC_4,    KC_5,    KC_6,    KC_PLUS, _______,
  _______, _______, _______, _______, _______, _______, _______,     _______, KC_AMPR, KC_1,    KC_2,    KC_3,    KC_BSLS, _______,
  _______, _______, _______, KC_BTN1, KC_BTN2,                                         KC_0, KC_COMMA,  KC_DOT,    KC_EQL,  _______,

                                               _______, _______,     XXXXXXX, _______,
                                                        _______,     _______,
                                      _______, _______, _______,     _______, _______, _______
),
/* Keymap 4: Gaming
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |------|           |------|      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[GAMING] = LAYOUT_ergodox_pretty(
  _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, TO(BASE),
  _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______,                       _______, _______, _______, _______, _______, _______,
  KC_LSFT, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, KC_RSFT,
  _______, _______, _______, _______, KC_SPACE,                                        _______, _______, _______, _______, _______,

                                               _______, _______,     _______, XXXXXXX,
                                                        _______,     _______,
                                      _______, _______, _______,     _______, _______, _______
),
};

//----------------------
// BLANK LAYER TEMPLATE
//----------------------
// 
/* Keymap X: <name>
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |------|           |------|      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
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
    // This will do most of the grunt work with the keycodes.
    switch (keycode) { 
        case KC_ESC:
            if (get_oneshot_mods() != 0)
            {
                clear_oneshot_mods();
            }
            break;
        case CMD_TAB:
            if (record->event.pressed) 
            {
                if (!is_cmd_tab_active) 
                {
                    is_cmd_tab_active = true;
                    register_code(KC_LCMD);
                }
                cmd_tab_timer = timer_read();
                register_code(KC_TAB);
            } 
            else 
            {
                unregister_code(KC_TAB);
            }
            break;
        case VRSN:
            SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
            return false;
        case COLON_EQUAL:
            if (record->event.pressed) 
                SEND_STRING(":=");
                
            break;
    }
    return true;
}

void matrix_scan_user(void) {
    if (is_cmd_tab_active) {
        if (timer_elapsed(cmd_tab_timer) > 1000) {
            unregister_code(KC_LCMD);
            is_cmd_tab_active = false;
        }
    }
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
            ergodox_right_led_3_on();
            break;
        default:
            break;
    }

    return state;
};
