#include QMK_KEYBOARD_H
#include "version.h"
#include "secrets.h"

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
};

enum layers {
    BASE,      // default layer
    SYMBOLS,
    MOUSE,
    NUMBERS,
    GAMING,
};

// Tap Dance declarations
enum td_keycodes {
    TD_DASH,
    TD_PLUS,
    TD_LBRACKET,
    TD_RBRACKET,
    TD_PIPE,
    TD_COPY,
    TD_UNDO,
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for first key code tap twice for second keycode
    [TD_DASH]     = ACTION_TAP_DANCE_DOUBLE(KC_MINUS, KC_UNDS),
    [TD_PLUS]     = ACTION_TAP_DANCE_DOUBLE(KC_EQUAL, KC_PLUS),
    [TD_LBRACKET] = ACTION_TAP_DANCE_DOUBLE(KC_LEFT_CURLY_BRACE, KC_LEFT_BRACKET),
    [TD_RBRACKET] = ACTION_TAP_DANCE_DOUBLE(KC_RIGHT_CURLY_BRACE, KC_RIGHT_BRACKET),
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
 * | LShift |Z/Ctrl|   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |//Ctrl| RShift |
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
    KC_ESCAPE,      KC_1,     KC_2,     KC_3,           KC_4,           KC_5,     TD(TD_DASH),           TD(TD_PLUS),      KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           KC_BSPC,
    KC_TAB,         KC_Q,     KC_W,     KC_E,           KC_R,           KC_T,     TD(TD_LBRACKET),       TD(TD_RBRACKET),  KC_Y,           KC_U,           KC_I,           KC_O,           KC_P,           TD(TD_PIPE),
    MO(SYMBOLS),    KC_A,     KC_S,     KC_D,           KC_F,           KC_G,                                              KC_H,           KC_J,           KC_K,           KC_L,           KC_SEMICOLON,   KC_QUOTE,
    SC_LSPO,        KC_Z,     KC_X,     KC_C,           KC_V,           KC_B,     KC_GRAVE,              CW_TOGG,          KC_N,           KC_M,           KC_COMMA,       KC_DOT,         KC_SLASH,       SC_RSPC,
    OSM(MOD_LCTL),  KC_TRNS,  KC_TRNS,  OSM(MOD_LALT),  OSM(MOD_LGUI),                                                                     TT(MOUSE),      TD(TD_UNDO),    TD(TD_COPY),    LGUI(KC_V),     LGUI(LSFT(KC_2)),
                                                                                  KC_TRNS,   KC_TRNS,    LALT(KC_SPACE),  LGUI(KC_SPACE),
                                                                                          TG(GAMING),    KC_TRNS, 
                                                                    KC_SPACE, KC_DELETE, TT(NUMBERS),    LGUI(KC_GRAVE), KC_ENTER, KC_BSPC

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
  KC_TRNS, KC_MPRV, KC_MPLY, KC_MNXT, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_HOME, KC_UP,   KC_END,   KC_TRNS, KC_TRNS,
  KC_TRNS, KC_VOLD, KC_VOLU, KC_TRNS, KC_TRNS, KC_TRNS,                       KC_TRNS, KC_LEFT, KC_DOWN, KC_RIGHT, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_CAPS, KC_MINS, KC_UNDS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LALT,                                         KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,

                                               RGB_TOG, RGB_MODE_PLAIN,     KC_TRNS, KC_TRNS,
                                                        TOGGLE_LAYER_COLOR,     KC_TRNS,
                                      QK_LEAD, KC_TRNS, KC_TRNS,      KC_TRNS, KC_LCTL, KC_LALT
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
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, QK_BOOT,
  KC_TRNS, KC_TRNS, KC_WH_U, KC_MS_U, KC_WH_D, KC_BTN3, KC_TRNS,     KC_TRNS, KC_TRNS, KC_WH_U, KC_MS_U, KC_WH_D, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_WH_L, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_R,                       KC_WH_L, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_R, KC_TRNS,
  KC_WFWD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_WFWD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_WBAK,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN1,                                         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

                                               KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS,
                                                        KC_TRNS,     KC_TRNS,
                                      KC_BTN2, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS
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
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, TO(BASE),
  KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_U, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_7,    KC_8,    KC_9,    KC_ASTR, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS,                       KC_TRNS, KC_4,    KC_5,    KC_6,    KC_PLUS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_AMPR, KC_1,    KC_2,    KC_3,    KC_BSLS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN1, KC_BTN2,                                         KC_0, KC_COMMA,  KC_DOT,    KC_EQL,  KC_TRNS,

                                               KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS,
                                                        KC_TRNS,     KC_TRNS,
                                      KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_WBAK
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
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, TO(BASE),
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_LSFT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_RSFT,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_SPACE,                                        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

                                               KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS,
                                                        KC_TRNS,     KC_TRNS,
                                      KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS
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
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

                                               KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS,
                                                        KC_TRNS,     KC_TRNS,
                                      KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS
),
*/
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case VRSN:
                SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
                return false;
        }
    }
    return true;
}

static bool is_leader_key_pressed = false;

void leader_start_user(void) {
    is_leader_key_pressed = true;

    ergodox_right_led_1_on();
    ergodox_right_led_2_on();
    ergodox_right_led_3_on();

    rgb_matrix_set_color_all(255, 255, 255);  // White
    rgb_matrix_set_color(IDX_C, 255, 0, 0);   // Light Blue
    rgb_matrix_set_color(IDX_V, 255, 0, 0);   // Light Blue
    rgb_matrix_set_color(IDX_1, 255, 0, 0);   // Light Blue
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
            ergodox_right_led_2_on();
            break;
        case NUMBERS:
            rgb_matrix_set_color_all(0, 0, 255); // Blue
            ergodox_right_led_3_on();
            break;
        case GAMING:
            rgb_matrix_set_color_all(255, 0, 0); // Red
            ergodox_right_led_1_on();
            ergodox_right_led_3_on();
            break;
        default:
            break;
    }

    return state;
};
