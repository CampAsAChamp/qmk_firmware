/*
 * campasachamp — ErgoDox EZ Glow keymap
 *
 * Layers:
 *   BASE_MAC / BASE_WIN     — home row, layer keys, OS-specific command mod-taps
 *   SHORTCUTS_MAC / _WIN    — copy/paste, word-delete, app switcher (LGUI vs LCTL)
 *   SYMBOLS, MEDIA, NUMBERS, MOUSE — shared; TO_HOME returns to active OS base layer
 *   GAMING — Windows WASD gaming layout (plain keys, no home-row mods)
 *
 * Cross-platform: see CROSS_PLATFORM.md — OS auto-detect, paired _MAC/_WIN layers,
 * manual toggle on MOUSE layer. Do not enable macOS System Settings Ctrl↔Cmd swap.
 */
#include QMK_KEYBOARD_H

#include "./key_indexes.h"
#include "os_detection.h"
#include "eeconfig.h"

// Modifier chord aliases (Hyper = Shift+Ctrl+Alt+Cmd, Meh = Shift+Ctrl+Alt)
// Placed on Z/X/. because they are rarely double-tapped in normal prose
#define MY_HYPER S(G(C(KC_LALT)))
#define MY_MEH S(C(KC_LALT))

#define MY_HYPER_X MT(MOD_LSFT | MOD_LGUI | MOD_LCTL | MOD_LALT, KC_X)
#define MY_MEH_Z MT(MOD_LSFT | MOD_LCTL | MOD_LALT, KC_Z)
// Z / X / period: tap = letter, hold = Meh or Hyper (shared across OS modes)
#define MY_HYPER_DOT MT(MOD_LSFT | MOD_LGUI | MOD_LCTL | MOD_LALT, KC_DOT)

#define TO_HOME MY_TO_BASE
enum layers {
    BASE_MAC,
    BASE_WIN,
    SHORTCUTS_MAC,
    SHORTCUTS_WIN,
    SYMBOLS, // shared — bracket/symbol keys on C/V row
    MEDIA,
    NUMBERS,
    MOUSE, // MY_OS_TOGGLE lives here (manual Mac/Win override)
    GAMING,
};

enum td_keycodes {
    TD_PIPE,
    TD_CAPS_BASIC,
};

enum custom_keycodes {
    SUPER_ALT_TAB = SAFE_RANGE, // hold modifier + Tab for app switcher (Cmd+Tab Mac, Ctrl+Tab Win)
    MY_OS_TOGGLE, // MOUSE layer: tap = lock Mac/Win, hold = unlock + re-detect
    MY_TO_BASE,   // TO_HOME — jump to BASE_MAC or BASE_WIN based on os_is_mac
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

tap_dance_action_t tap_dance_actions[] = {
    [TD_PIPE] = ACTION_TAP_DANCE_DOUBLE(KC_BACKSLASH, KC_PIPE), // on P: \ tap, | double-tap
    [TD_CAPS_BASIC] = ACTION_TAP_DANCE_FN(dance_caps), // 1=tap shift, 2=caps word, 3=caps lock
};

// G + H (both index fingers on home row) → Hyper+Enter
const uint16_t PROGMEM HOMEROW_APP_ACTIVATION[] = {KC_G, KC_H, COMBO_END};
combo_t key_combos[] = {
    COMBO(HOMEROW_APP_ACTIVATION, HYPR(KC_ENTER)),
};

// SUPER_ALT_TAB: modifier held until 750 ms idle, then released in matrix_scan_user
bool     is_alt_tab_active = false;
uint16_t alt_tab_timer     = 0;

// --- Cross-platform OS mode (Mode A only) ---
// Auto-detect on USB connect/switch; manual_os_locked skips process_detected_host_os_user.
// EEPROM (config.h EECONFIG_USER_DATA_SIZE) persists manual lock + last OS across power cycles.
bool     manual_os_locked  = false;
bool     os_is_mac         = false;
uint16_t os_toggle_timer   = 0;
uint16_t os_rgb_timer      = 0;
bool     os_rgb_active     = false;
static bool suppress_mouse_layer = false;

typedef union {
    uint32_t raw;
    struct {
        uint8_t os_is_mac;
        uint8_t manual_lock;
        uint8_t reserved[2];
    };
} user_config_t;

static user_config_t user_config;

static bool is_mac_os(void) {
    return os_is_mac;
}

static void save_user_config(void) {
    user_config.os_is_mac   = os_is_mac ? 1 : 0;
    user_config.manual_lock = manual_os_locked ? 1 : 0;
    eeconfig_update_user_datablock(&user_config, 0, sizeof(user_config));
}

static void load_user_config(void) {
    if (eeconfig_is_user_datablock_valid()) {
        eeconfig_read_user_datablock(&user_config, 0, sizeof(user_config));
    }
}

// Brief full-keyboard flash after detect/toggle: white = Mac, blue = Windows (~300 ms)
static void flash_os_rgb(bool mac) {
    if (mac) {
        rgb_matrix_set_color_all(255, 255, 255);
    } else {
        rgb_matrix_set_color_all(0, 0, 255);
    }
    os_rgb_timer  = timer_read();
    os_rgb_active = true;
}

// Switch default layer to BASE_MAC or BASE_WIN and persist choice
static void apply_os_layer(bool mac) {
    os_is_mac = mac;
    if (layer_state_is(MOUSE)) {
        suppress_mouse_layer = true;
    }
    layer_clear();
    set_single_persistent_default_layer(mac ? BASE_MAC : BASE_WIN);
    flash_os_rgb(mac);
    save_user_config();
}

static void apply_os_from_detection(void) {
    os_variant_t detected = detected_host_os();
    apply_os_layer(detected == OS_MACOS || detected == OS_IOS);
}

void eeconfig_init_user(void) {
    user_config.raw = 0;
    eeconfig_update_user_datablock(&user_config, 0, sizeof(user_config));
}

// QMK calls this when the host OS is identified (USB enumerate / switcher re-connect)
bool process_detected_host_os_user(os_variant_t detected_os) {
    if (manual_os_locked) {
        return true;
    }
    apply_os_layer(detected_os == OS_MACOS || detected_os == OS_IOS);
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (suppress_mouse_layer) {
        state &= ~((layer_state_t)1 << MOUSE);
    }
    return state;
}

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

/*
 * BASE_MAC — Mac default layer (selected by OS detect or manual toggle)
 * Same physical layout as BASE_WIN; only OS-specific modifiers and layer targets differ.
 * A/; = LCTL_T (terminal/emacs); D/K = LCMD_T (primary Mac shortcut modifier).
 */
[BASE_MAC] = LAYOUT_ergodox_pretty(
         KC_GRV,         KC_1,         KC_2,             KC_3,          KC_4,           KC_5,          KC_UNDS,      KC_PLUS             , KC_6       , KC_7        , KC_8                , KC_9        , KC_0                 , KC_EQUAL         ,
            KC_TAB,         KC_Q,         KC_W,             KC_E,          KC_R,           KC_T,          HYPR(KC_ENTER),      KC_TILDE            , KC_Y       , KC_U        , KC_I                , KC_O        , KC_P                 , TD(TD_PIPE)      ,
  LT(MEDIA,KC_ESC), LCTL_T(KC_A), LALT_T(KC_S),     LCMD_T(KC_D),  LSFT_T(KC_F),           KC_G,                                              KC_H       , RSFT_T(KC_J), LCMD_T(KC_K)        , LALT_T(KC_L), LCTL_T(KC_SEMICOLON) , KC_QUOTE         ,
 TD(TD_CAPS_BASIC),     MY_MEH_Z,   MY_HYPER_X, LT(SYMBOLS,KC_C),          KC_V,           KC_B,   LCMD(KC_SPACE),      HYPR(KC_ENTER)       , KC_N       , KC_M        , LT(SYMBOLS,KC_COMMA), MY_HYPER_DOT, MT(MOD_RGUI,KC_SLASH), TD(TD_CAPS_BASIC),
TOGGLE_LAYER_COLOR,      _______,      _______,          _______, MO(SHORTCUTS_MAC),                                                                           TT(MOUSE)   , KC_LEFT             , KC_UP     , KC_DOWN              , KC_RIGHT          ,

                                                                                 LALT(KC_SPACE), LCMD(LSFT(KC_1)),      _______             , TT(NUMBERS),
                                                                                                 LCMD(LSFT(KC_2)),      _______             ,
  // Cmd+Ctrl+Space = emoji/special-char picker on Mac; Delete (not Bksp) = forward delete on Mac keyboards
                                                                       KC_SPACE,      KC_DELETE, LCMD(LSFT(KC_5)),      LCMD(LCTL(KC_SPACE)), KC_ENTER   , KC_BSPC
),

/*
 * BASE_WIN — Windows default layer
 * A/; = LGUI_T (Win key); D/K = LCTL_T (Ctrl shortcuts); S = LCTL_T for word-nav with SHORTCUTS arrows.
 */
[BASE_WIN] = LAYOUT_ergodox_pretty(
         KC_GRV,         KC_1,         KC_2,             KC_3,          KC_4,           KC_5,          KC_UNDS,      KC_PLUS             , KC_6       , KC_7        , KC_8                , KC_9        , KC_0                 , KC_EQUAL         ,
            KC_TAB,         KC_Q,         KC_W,             KC_E,          KC_R,           KC_T,          HYPR(KC_ENTER),      KC_TILDE            , KC_Y       , KC_U        , KC_I                , KC_O        , KC_P                 , TD(TD_PIPE)      ,
  LT(MEDIA,KC_ESC), LGUI_T(KC_A), LCTL_T(KC_S),     LCTL_T(KC_D),  LSFT_T(KC_F),           KC_G,                                              KC_H       , RSFT_T(KC_J), LCTL_T(KC_K)        , LALT_T(KC_L), LGUI_T(KC_SEMICOLON) , KC_QUOTE         ,
 TD(TD_CAPS_BASIC),     MY_MEH_Z,   MY_HYPER_X, LT(SYMBOLS,KC_C),          KC_V,           KC_B,   LCTL(KC_SPACE),      HYPR(KC_ENTER)       , KC_N       , KC_M        , LT(SYMBOLS,KC_COMMA), MY_HYPER_DOT, MT(MOD_RGUI,KC_SLASH), TD(TD_CAPS_BASIC),
TOGGLE_LAYER_COLOR,      _______,      _______,          _______, MO(SHORTCUTS_WIN),                                                                           TT(MOUSE)   , KC_LEFT             , KC_UP     , KC_DOWN              , KC_RIGHT          ,

                                                                                 LALT(KC_SPACE), LCTL(LSFT(KC_1)),      TG(GAMING)          , TT(NUMBERS),
                                                                                                 LCTL(LSFT(KC_2)),      _______             ,
                                                                       KC_SPACE,      KC_DELETE, LCTL(LSFT(KC_5)),      LGUI(KC_S), KC_ENTER   , KC_BSPC
),

/*
 * SHORTCUTS_MAC — momentary; hold MO(SHORTCUTS_MAC) on base
 * LGUI row = native Mac app/window shortcuts; LALT(KC_BSPC) = Option+Backspace word-delete.
 */
[SHORTCUTS_MAC] = LAYOUT_ergodox_pretty(
 LGUI(KC_GRV),      KC_F1,      KC_F2,      KC_F3,      KC_F4,   KC_F5,  KC_F11,      KC_F12 , KC_F6         , KC_F7        , KC_F8  , KC_F9   , KC_F10 , TO(BASE_MAC),
SUPER_ALT_TAB, LGUI(KC_Q), LGUI(KC_W),    _______, LGUI(KC_R), LGUI(KC_T), _______,      _______, _______       , KC_HOME      , KC_UP  , KC_END  , _______, _______ ,
_______,    _______,    _______,    _______,    _______, _______,                        _______       , KC_LEFT      , KC_DOWN, KC_RIGHT, _______, KC_GRAVE,
      _______, LGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), LGUI(KC_V), _______, _______,      KC_SLEP, KC_MINS       , KC_UNDS      , _______, _______ , _______, _______ ,
      _______,    _______,    _______,    _______,    _______,                                                 _______      , _______, _______ , _______, _______ ,

                                                               _______, _______,      _______, _______       ,
                                                                        _______,      _______,
  // LALT+Bksp = delete previous word on macOS (different modifier than copy/paste row)
                                                      _______, _______, _______,      _______, LGUI(KC_ENTER), LALT(KC_BSPC)
),

/*
 * SHORTCUTS_WIN — same layout as SHORTCUTS_MAC with LCTL instead of LGUI
 * LCTL(KC_BSPC) = Ctrl+Backspace word-delete on Windows.
 */
[SHORTCUTS_WIN] = LAYOUT_ergodox_pretty(
 LCTL(KC_GRV),      KC_F1,      KC_F2,      KC_F3,      KC_F4,   KC_F5,  KC_F11,      KC_F12 , KC_F6         , KC_F7        , KC_F8  , KC_F9   , KC_F10 , TO(BASE_WIN),
SUPER_ALT_TAB, LCTL(KC_Q), LCTL(KC_W),    _______, LCTL(KC_R), LCTL(KC_T), _______,      _______, _______       , KC_HOME      , KC_UP  , KC_END  , _______, _______ ,
 LCTL(KC_GRV),    _______,    _______,    _______,    _______, _______,                        _______       , KC_LEFT      , KC_DOWN, KC_RIGHT, _______, KC_GRAVE,
      _______, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), _______, _______,      KC_SLEP, KC_MINS       , KC_UNDS      , _______, _______ , _______, _______ ,
      _______,    _______,    _______,    _______,    _______,                                                 _______      , _______, _______ , _______, _______ ,

                                                               _______, _______,      _______, _______       ,
                                                                        _______,      _______,
                                                      _______, _______, _______,      _______, LCTL(KC_ENTER), LCTL(KC_BSPC)
),

// Shared layers: OS-neutral keys; TO_HOME (MY_TO_BASE) returns to whichever BASE_* is active
// Bracket layout mirrors physical C/V/D/F/comma/period positions from base layer

[SYMBOLS] = LAYOUT_ergodox_pretty(
_______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______,
_______, _______, _______, KC_LCBR, KC_RCBR, _______, _______,      _______, _______, _______, _______, _______, _______, _______,
_______, _______, _______, KC_LPRN, KC_RPRN, _______,                        _______, _______, _______, _______, _______, _______,
_______, _______, _______, KC_LBRC, KC_RBRC, _______, _______,      _______, _______, _______, _______, _______, _______, _______,
_______, _______, _______, KC_LABK, KC_RABK,                                          _______, _______, _______, _______, _______,

                                             _______, _______,      _______, _______,
                                                      _______,      _______,
                                    _______, _______, _______,      _______, _______, _______
),

// TO_HOME on exit: returns to BASE_MAC or BASE_WIN without hard-coding either layer name
[MEDIA] = LAYOUT_ergodox_pretty(
LCTL(KC_GRV),   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,             KC_F11,      KC_F12 , KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , TO_HOME,
     _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, _______,            _______,      _______, _______, _______, _______, _______, _______, _______ ,
     _______, _______, KC_VOLD, KC_VOLU, _______, _______,                                   _______, _______, _______, _______, _______, _______ ,
     _______, _______, _______, _______, _______, _______,            _______,      _______, _______, _______, _______, _______, _______, _______ ,
     _______, _______, _______, _______, _______,                                                     _______, _______, _______, _______, _______ ,

                                                  _______,     RGB_MODE_PLAIN,      _______, _______,
                                                           TOGGLE_LAYER_COLOR,      _______,
                                         _______, _______,            _______,      _______, _______, _______
),

[NUMBERS] = LAYOUT_ergodox_pretty(
_______, _______, _______, _______,  _______, _______, _______,      _______, _______, _______, _______ , _______, _______, TO_HOME,
_______, _______, _______,   KC_UP,  _______, _______, _______,      _______, _______, KC_7   , KC_8    , KC_9   , KC_ASTR, _______ ,
_______, _______, KC_LEFT, KC_DOWN, KC_RIGHT, _______,                        _______, KC_4   , KC_5    , KC_6   , KC_PLUS, _______ ,
_______, _______, _______, _______,  _______, _______, _______,      _______, _______, KC_1   , KC_2    , KC_3   , KC_BSLS, _______ ,
_______, _______, _______, _______,  _______,                                          KC_0   , KC_COMMA, KC_DOT , KC_EQL , _______ ,

                                              _______, _______,      XXXXXXX, _______,
                                                       _______,      _______,
                                     _______, _______, _______,      _______, _______, _______
),

/*
 * MOUSE — same as Mode B, plus MY_OS_TOGGLE on the Tab key
 * Infrequent layer = safe place for OS override (tap flip Mac/Win, hold re-detect).
 * See CROSS_PLATFORM.md for USB switcher fallback behavior.
 */
[MOUSE] = LAYOUT_ergodox_pretty(
_______, MS_ACL0, MS_ACL1, MS_ACL2, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______,
MY_OS_TOGGLE, _______, MS_WHLU,   MS_UP, MS_WHLD, _______, _______,      _______, _______, _______, _______, _______, _______, QK_BOOT,
_______, MS_WHLL, MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLL,                        _______, _______, _______, _______, _______, _______,
MS_BTN4, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, MS_BTN5,
_______, _______, _______, _______, MS_BTN1,                                          _______, _______, _______, _______, _______,

                                             _______, _______,      _______, _______,
                                                      _______,      _______,
                                    MS_BTN2, MS_BTN3, _______,      _______, _______, _______
),

// Windows-only gaming layer — plain WASD, LWIN bottom row, SHORTCUTS_WIN on space hold
[GAMING] = LAYOUT_ergodox_pretty(
KC_TILDE, _______, _______, _______,  _______, _______,     _______,      _______, _______, _______, _______, _______, _______     , TO(BASE_WIN),
 _______, _______, _______, _______,  _______, _______,     _______,      _______, _______, _______, _______, _______, _______     , _______ ,
 LT(MEDIA,KC_ESC),    KC_A,    KC_S,    KC_D,     KC_F, _______,                            _______, KC_J   , KC_K   , KC_L   , KC_SEMICOLON, _______ ,
 KC_LSFT, _______, _______, _______,  _______, _______, MO(SYMBOLS),      _______, _______, _______, _______, _______, _______     , KC_RSFT ,
 KC_LCTL, KC_LALT, KC_LWIN, XXXXXXX, KC_SPACE,                                              _______, KC_LEFT, KC_UP  , KC_DOWN     , KC_RIGHT,

                                               _______,     _______,      _______, XXXXXXX,
                                               KC_VOLU,      _______,
                                      LT(SHORTCUTS_WIN,KC_SPACE), _______, KC_VOLD,      _______, _______, _______
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

void keyboard_post_init_user(void) {
    rgb_matrix_enable();
    // Restore manual OS lock from EEPROM, else wait for / run auto-detect
    load_user_config();
    if (user_config.manual_lock) {
        manual_os_locked = true;
        apply_os_layer(user_config.os_is_mac);
    } else {
        apply_os_from_detection();
    }
}

// Per-layer RGB helpers — LED indices from key_indexes.h
static void rgb_indicators_base(void) {
    // Mac: purple base fill; Windows: blue base fill (green/orange/red accents unchanged)
    if (os_is_mac) {
        rgb_matrix_set_color_all(97, 0, 255);
    } else {
        rgb_matrix_set_color_all(0, 0, 255);
    }
    rgb_matrix_set_color(IDX_Z, 23, 200, 34);
    rgb_matrix_set_color(IDX_X, 23, 200, 34);
    rgb_matrix_set_color(IDX_C, 23, 200, 34);
    rgb_matrix_set_color(IDX_Comma, 23, 200, 34);
    rgb_matrix_set_color(IDX_Period, 23, 200, 34);
    rgb_matrix_set_color(IDX_F_Slash, 23, 200, 34);
    rgb_matrix_set_color(IDX_G, 255, 149, 0);
    rgb_matrix_set_color(IDX_H, 255, 149, 0);
    rgb_matrix_set_color(IDX_L4, 255, 0, 0);
    rgb_matrix_set_color(IDX_R1, 255, 0, 0);
    // White accent on thumb cluster when manual OS lock is active
    if (manual_os_locked) {
        rgb_matrix_set_color(IDX_R2, 255, 255, 255);
        rgb_matrix_set_color(IDX_R3, 255, 255, 255);
    }
}

static void rgb_indicators_shortcuts(void) {
    rgb_matrix_set_color_all(0, 0, 0);
    rgb_matrix_set_color(IDX_1, 255, 0, 0);
    rgb_matrix_set_color(IDX_2, 255, 0, 0);
    rgb_matrix_set_color(IDX_4, 255, 0, 0);
    rgb_matrix_set_color(IDX_5, 255, 0, 0);
    rgb_matrix_set_color(IDX_6, 255, 0, 0);
    rgb_matrix_set_color(IDX_7, 255, 0, 0);
    rgb_matrix_set_color(IDX_8, 255, 0, 0);
    rgb_matrix_set_color(IDX_9, 255, 0, 0);
    rgb_matrix_set_color(IDX_0, 255, 0, 0);
    rgb_matrix_set_color(IDX_Q, 0, 0, 255);
    rgb_matrix_set_color(IDX_W, 0, 0, 255);
    rgb_matrix_set_color(IDX_R, 0, 0, 255);
    rgb_matrix_set_color(IDX_Z, 0, 0, 255);
    rgb_matrix_set_color(IDX_X, 0, 0, 255);
    rgb_matrix_set_color(IDX_C, 0, 0, 255);
    rgb_matrix_set_color(IDX_V, 0, 0, 255);
    rgb_matrix_set_color(IDX_I, 255, 255, 255);
    rgb_matrix_set_color(IDX_J, 255, 255, 255);
    rgb_matrix_set_color(IDX_K, 255, 255, 255);
    rgb_matrix_set_color(IDX_L, 255, 255, 255);
    rgb_matrix_set_color(IDX_U, 255, 149, 0);
    rgb_matrix_set_color(IDX_O, 255, 149, 0);
    rgb_matrix_set_color(IDX_N, 255, 0, 255);
    rgb_matrix_set_color(IDX_M, 255, 0, 255);
}

static void rgb_indicators_gaming(void) {
    rgb_matrix_set_color_all(0, 0, 255);
    rgb_matrix_set_color(IDX_L4, 255, 255, 255);
}

bool rgb_matrix_indicators_user(void) {
    if (keyboard_config.disable_layer_led) { return false; }

    // Suppress layer colors briefly after OS detect/toggle flash
    if (os_rgb_active) {
        return true;
    }

    switch (get_highest_layer(layer_state)) {
        case BASE_MAC:
        case BASE_WIN:
            rgb_indicators_base();
            break;

        case SHORTCUTS_MAC:
        case SHORTCUTS_WIN:
            rgb_indicators_shortcuts();
            break;

        case SYMBOLS:
            rgb_matrix_set_color_all(0, 0, 0);
            rgb_matrix_set_color(IDX_E, 255, 0, 0);
            rgb_matrix_set_color(IDX_R, 255, 0, 0);
            rgb_matrix_set_color(IDX_D, 255, 255, 255);
            rgb_matrix_set_color(IDX_F, 255, 255, 255);
            rgb_matrix_set_color(IDX_C, 0, 0, 255);
            rgb_matrix_set_color(IDX_V, 0, 0, 255);
            break;

        case MEDIA:
            rgb_matrix_set_color_all(0, 0, 0);
            rgb_matrix_set_color(IDX_1, 0, 0, 255);
            rgb_matrix_set_color(IDX_2, 0, 0, 255);
            rgb_matrix_set_color(IDX_3, 0, 0, 255);
            rgb_matrix_set_color(IDX_4, 0, 0, 255);
            rgb_matrix_set_color(IDX_5, 0, 0, 255);
            rgb_matrix_set_color(IDX_6, 0, 0, 255);
            rgb_matrix_set_color(IDX_7, 0, 0, 255);
            rgb_matrix_set_color(IDX_8, 0, 0, 255);
            rgb_matrix_set_color(IDX_9, 0, 0, 255);
            rgb_matrix_set_color(IDX_0, 0, 0, 255);
            rgb_matrix_set_color(IDX_Q, 23, 200, 34);
            rgb_matrix_set_color(IDX_W, 23, 200, 34);
            rgb_matrix_set_color(IDX_E, 23, 200, 34);
            rgb_matrix_set_color(IDX_S, 97, 0, 255);
            rgb_matrix_set_color(IDX_D, 97, 0, 255);
            break;

        case NUMBERS:
            rgb_matrix_set_color_all(0, 0, 0);
            rgb_matrix_set_color(IDX_U, 255, 149, 0);
            rgb_matrix_set_color(IDX_I, 255, 149, 0);
            rgb_matrix_set_color(IDX_O, 255, 149, 0);
            rgb_matrix_set_color(IDX_J, 255, 149, 0);
            rgb_matrix_set_color(IDX_K, 255, 149, 0);
            rgb_matrix_set_color(IDX_L, 255, 149, 0);
            rgb_matrix_set_color(IDX_M, 255, 149, 0);
            rgb_matrix_set_color(IDX_Comma, 255, 149, 0);
            rgb_matrix_set_color(IDX_Period, 255, 149, 0);
            rgb_matrix_set_color(IDX_R1, 255, 149, 0);
            rgb_matrix_set_color(IDX_R2, 255, 0, 0);
            rgb_matrix_set_color(IDX_R3, 255, 0, 0);
            rgb_matrix_set_color(IDX_P, 23, 200, 34);
            rgb_matrix_set_color(IDX_Colon, 23, 200, 34);
            rgb_matrix_set_color(IDX_F_Slash, 23, 200, 34);
            rgb_matrix_set_color(IDX_R4, 23, 200, 34);
            rgb_matrix_set_color(IDX_E, 255, 255, 255);
            rgb_matrix_set_color(IDX_S, 255, 255, 255);
            rgb_matrix_set_color(IDX_D, 255, 255, 255);
            rgb_matrix_set_color(IDX_F, 255, 255, 255);
            break;

        case MOUSE:
            rgb_matrix_set_color_all(0, 0, 0);
            rgb_matrix_set_color(IDX_E, 255, 255, 255);
            rgb_matrix_set_color(IDX_S, 255, 255, 255);
            rgb_matrix_set_color(IDX_D, 255, 255, 255);
            rgb_matrix_set_color(IDX_F, 255, 255, 255);
            rgb_matrix_set_color(IDX_1, 255, 0, 0);
            rgb_matrix_set_color(IDX_2, 255, 255, 0);
            rgb_matrix_set_color(IDX_3, 23, 200, 34);
            rgb_matrix_set_color(IDX_W, 0, 0, 255);
            rgb_matrix_set_color(IDX_R, 0, 0, 255);
            rgb_matrix_set_color(IDX_A, 0, 0, 255);
            rgb_matrix_set_color(IDX_G, 0, 0, 255);
            rgb_matrix_set_color(IDX_L4, 97, 0, 255);
            // OS toggle key: white = Mac mode, blue = Windows mode
            rgb_matrix_set_color(IDX_R2, os_is_mac ? 255 : 0, os_is_mac ? 255 : 0, os_is_mac ? 255 : 255);
            break;

        case GAMING:
            rgb_indicators_gaming();
            break;

        default:
            if (rgb_matrix_get_flags() == LED_FLAG_NONE)
                rgb_matrix_set_color_all(0, 0, 0);
            break;
    }

  return true;
}

// clang-format on
// Make - not turn into _ when CAPS_WORD is on
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Letters: continue Caps Word with shift applied.
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT));
            return true;

        // Continue Caps Word without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
        case KC_MINS:
            return true;

        default:
            return false;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (IS_QK_LAYER_TAP_TOGGLE(keycode) && QK_LAYER_TAP_TOGGLE_GET_LAYER(keycode) == MOUSE) {
        if (!record->event.pressed) {
            suppress_mouse_layer = false;
        }
    }

    switch (keycode) {
        case KC_ESC:
            if (get_oneshot_mods() != 0)
                clear_oneshot_mods();

            break;
        case SUPER_ALT_TAB:
            // App switcher: register Cmd or Ctrl once, then Tab on each press; release in matrix_scan_user
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(is_mac_os() ? KC_LGUI : KC_LEFT_CTRL);
                }

                alt_tab_timer = timer_read();
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }

            return false;
        case MY_TO_BASE:
            // Used by TO_HOME on shared layers (SYMBOLS, MEDIA, NUMBERS)
            if (record->event.pressed) {
                layer_clear();
                set_single_persistent_default_layer(os_is_mac ? BASE_MAC : BASE_WIN);
            }
            return false;
        case MY_OS_TOGGLE:
            // Tap: flip Mac↔Win and lock. Hold past TAPPING_TERM: unlock and re-detect host OS.
            if (record->event.pressed) {
                os_toggle_timer = timer_read();
            } else {
                if (timer_elapsed(os_toggle_timer) > TAPPING_TERM) {
                    manual_os_locked = false;
                    apply_os_from_detection();
                } else {
                    manual_os_locked = true;
                    apply_os_layer(!os_is_mac);
                }
            }
            return false;
    }
    return true;
}

void matrix_scan_user(void) {
    // End full-keyboard OS flash after ~300 ms
    if (os_rgb_active && timer_elapsed(os_rgb_timer) > 300) {
        os_rgb_active = false;
    }

    // Release Cmd/Ctrl after SUPER_ALT_TAB idle timeout
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 750) {
            unregister_code(is_mac_os() ? KC_LGUI : KC_LEFT_CTRL);
            is_alt_tab_active = false;
        }
    }
}
