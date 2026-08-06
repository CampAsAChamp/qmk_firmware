/*
 * campasachamp — ErgoDox EZ Glow keymap
 *
 * Layers (Mode A, default flash):
 *   BASE_MAC / BASE_WIN     — home row, layer keys, OS-specific command mod-taps
 *   SHORTCUTS_MAC / _WIN    — copy/paste, word-delete, app switcher (LGUI vs LCTL)
 *   SYMBOLS, MEDIA, NUMBERS, MOUSE — shared; TO_HOME returns to active OS base layer
 *   GAMING_MAC / GAMING_WIN — WASD gaming layout with OS-matched shortcut layer
 *
 * Cross-platform: see CROSS_PLATFORM.md. Two build modes (rules.mk):
 *   Mode A (default) — OS_DETECTION_ENABLE, paired _MAC/_WIN layers, manual toggle
 *   Mode B           — make KANATA_MODE=yes; single Win-convention stack; Kanata remaps on Mac
 *
 * Do not enable macOS System Settings Ctrl↔Cmd swap for this keyboard with either mode.
 */
#include QMK_KEYBOARD_H

#include "./key_indexes.h"
#ifndef KANATA_MODE
#    include "os_detection.h"
#    include "eeconfig.h"
#endif
// #include "../../../users/campasachamp/campasachamp.h"
// #include "../../../users/campasachamp/features/sentence_case.h"

// Modifier chord aliases (Hyper = Shift+Ctrl+Alt+Cmd, Meh = Shift+Ctrl+Alt)
// Placed on Z/X/. because they are rarely double-tapped in normal prose
#define MY_HYPER S(G(C(KC_LALT)))
#define MY_MEH S(G(KC_LALT))

#define MY_HYPER_X MT(MOD_LSFT | MOD_LGUI | MOD_LCTL | MOD_LALT, KC_X)
#define MY_MEH_Z MT(MOD_LSFT | MOD_LGUI | MOD_LALT, KC_Z)
// Z / X / period: tap = letter, hold = Meh or Hyper (shared across OS modes)
#define MY_HYPER_DOT MT(MOD_LSFT | MOD_LGUI | MOD_LCTL | MOD_LALT, KC_DOT)

#ifdef KANATA_MODE
// Mode B: one layer stack, Windows key conventions; Kanata handles Mac on the host.
#    define TO_HOME TO(BASE)
enum layers {
    BASE,
    SHORTCUTS,
    SYMBOLS,
    MEDIA,
    NUMBERS,
    MOUSE,
    GAMING,
};
#else
// Mode A: paired OS layers; shared layers use MY_TO_BASE to land on the active default.
#    define TO_HOME MY_TO_BASE
enum layers {
    BASE_MAC,
    BASE_WIN,
    SHORTCUTS_MAC,
    SHORTCUTS_WIN,
    SYMBOLS, // shared — bracket/symbol keys on C/V row
    MEDIA,
    NUMBERS,
    MOUSE, // MY_OS_TOGGLE lives here (manual Mac/Win override)
    GAMING_MAC,
    GAMING_WIN,
};
#endif

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
    SUPER_ALT_TAB = SAFE_RANGE, // hold modifier + Tab for app switcher (Cmd+Tab Mac, Ctrl+Tab Win)
    RGB_SLD,
    COLON_EQUAL, // types ":=" for Go / assignment-style editing
#ifndef KANATA_MODE
    MY_OS_TOGGLE, // MOUSE layer: tap = lock Mac/Win, hold = unlock + re-detect
    MY_TO_BASE,   // TO_HOME — jump to BASE_MAC or BASE_WIN based on os_is_mac
#endif
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
    // Tap dances on punctuation / outer keys — avoids letter-key double-tap conflicts (see cur_dance notes below)
    [TD_DASH] = ACTION_TAP_DANCE_DOUBLE(KC_MINUS, KC_UNDS),
    [TD_PLUS] = ACTION_TAP_DANCE_DOUBLE(KC_EQUAL, KC_PLUS),
    [TD_LBRACKET] = ACTION_TAP_DANCE_FN(dance_l_bracket),
    [TD_RBRACKET] = ACTION_TAP_DANCE_FN(dance_r_bracket),
    [TD_PIPE] = ACTION_TAP_DANCE_DOUBLE(KC_BACKSLASH, KC_PIPE), // on P: \ tap, | hold/double
    [TD_COPY] = ACTION_TAP_DANCE_DOUBLE(LGUI(KC_C), LGUI(KC_X)), // copy tap, cut double (legacy; shortcuts layer preferred)
    [TD_UNDO] = ACTION_TAP_DANCE_DOUBLE(LGUI(KC_Z), LGUI(LSFT(KC_Z))),
    [TD_CAPS_BASIC] = ACTION_TAP_DANCE_FN(dance_caps), // 1=tap shift, 2=caps word, 3=caps lock
    [TD_CAPS_ADVANCED] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_caps_finished, dance_caps_reset)

};

// G + H (both index fingers on home row) → Hyper+Enter
const uint16_t PROGMEM HOMEROW_APP_ACTIVATION[] = {KC_G, KC_H, COMBO_END};
combo_t key_combos[] = {
    COMBO(HOMEROW_APP_ACTIVATION, HYPR(KC_ENTER)),
};

// SUPER_ALT_TAB: modifier held until 750 ms idle, then released in matrix_scan_user
bool     is_alt_tab_active = false;
uint16_t alt_tab_timer     = 0;

#ifndef KANATA_MODE
// --- Cross-platform OS mode (Mode A only) ---
// Auto-detect on USB connect/switch; manual_os_locked skips process_detected_host_os_user.
// EEPROM (config.h EECONFIG_USER_DATA_SIZE) persists manual lock + last OS across power cycles.
bool     manual_os_locked  = false;
bool     os_is_mac         = false;
uint16_t os_toggle_timer   = 0;
uint16_t os_rgb_timer      = 0;
bool     os_rgb_active     = false;

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
#endif // KANATA_MODE

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

#ifdef KANATA_MODE
// ----- Mode B keymaps: Windows conventions throughout; Kanata remaps on macOS hosts -----

/*
 * BASE — daily typing layer
 * Home-row mods keep modifiers under fingers; outer keys hold tap dances and layer toggles.
 * Symmetric LT(SYMBOLS) on C and comma: brackets without a dedicated symbols layer key.
 */
[BASE] = LAYOUT_ergodox_pretty(
  // Top row: standard numbers; outer = keeps both pinkies on familiar -/= keys
         KC_GRV,         KC_1,         KC_2,             KC_3,          KC_4,           KC_5,          KC_MINUS,     KC_EQUAL            , KC_6       , KC_7        , KC_8                , KC_9        , KC_0                 , KC_EQUAL         ,
  // QWERTY + [ ] under T/Y: reach brackets while coding without SYMBOLS layer
            KC_TAB,         KC_Q,         KC_W,             KC_E,          KC_R,           KC_T,          KC_LBRC,      KC_RBRC             , KC_Y       , KC_U        , KC_I                , KC_O        , KC_P                 , TD(TD_PIPE)      ,
  // Home row: Esc/Media on pinky; A/; = primary OS command mod (Ctrl here, Kanata→Cmd on Mac)
  // D/K stay Ctrl for terminal/editor habits; F/J = Shift for natural roll-out typing
  LT(MEDIA,KC_ESC), LCTL_T(KC_A), LALT_T(KC_S),     LCTL_T(KC_D),  LSFT_T(KC_F),           KC_G,                                              KC_H       , RSFT_T(KC_J), LCTL_T(KC_K)        , LALT_T(KC_L), LCTL_T(KC_SEMICOLON) , KC_QUOTE         ,
  // Z/X/. = Meh/Hyper holds (low double-tap conflict); C/comma = SYMBOLS; B = Ctrl+Space (Spotlight/search)
  // Outer TD_CAPS: shift/caps-word without a dedicated Caps Lock key
 TD(TD_CAPS_BASIC),     MY_MEH_Z,   MY_HYPER_X, LT(SYMBOLS,KC_C),          KC_V,           KC_B,   LCTL(KC_SPACE),      KC_TILDE             , KC_N       , KC_M        , LT(SYMBOLS,KC_COMMA), MY_HYPER_DOT, MT(MOD_RCTL,KC_SLASH), TD(TD_CAPS_BASIC),
  // Left: RGB toggle + momentary SHORTCUTS. Right: toggle MOUSE + arrow cluster for one-handed nav
TOGGLE_LAYER_COLOR,      _______,      _______,          _______, MO(SHORTCUTS),                                                                           TT(MOUSE)   , KC_LEFT             , KC_UP     , KC_DOWN              , KC_RIGHT          ,

  // Thumbs: gaming/numbers toggles; center cluster = Space, word-forward Delete, IDE chords, Win+S search, Enter, Bksp
                                                                                 LALT(KC_SPACE), LCTL(LSFT(KC_1)),      TG(GAMING)          , TT(NUMBERS),
                                                                                                 LCTL(LSFT(KC_2)),      _______             ,
                                                                       KC_SPACE,      KC_DELETE, LCTL(LSFT(KC_5)),      LGUI(KC_S), KC_ENTER   , KC_BSPC
),

/*
 * SHORTCUTS — momentary layer (hold MO(SHORTCUTS) on base)
 * Left = window/app chords under QWERTY home positions; right = nav + edit row.
 * Thumb LCTL+Bksp = delete previous word (Kanata maps to Opt+Bksp on Mac).
 */
[SHORTCUTS] = LAYOUT_ergodox_pretty(
  // F-row + LCTL+` (task switch / window mgmt on Win; Kanata maps on Mac)
 LCTL(KC_GRV),      KC_F1,      KC_F2,      KC_F3,      KC_F4,   KC_F5,  KC_F11,      KC_F12 , KC_F6         , KC_F7        , KC_F8  , KC_F9   , KC_F10 , TO(BASE),
  // Q/W/R/T column = close tab, close window, reload, new tab — same keys as browser muscle memory
SUPER_ALT_TAB, LCTL(KC_Q), LCTL(KC_W),    _______, LCTL(KC_R), LCTL(KC_T), _______,      _______, _______       , KC_HOME      , KC_UP  , KC_END  , _______, _______ ,
  // Arrow cluster on right home area for doc/spreadsheet nav without moving to MOUSE layer
 LCTL(KC_GRV),    _______,    _______,    _______,    _______, _______,                        _______       , KC_LEFT      , KC_DOWN, KC_RIGHT, _______, KC_GRAVE,
  // Z/X/C/V = undo/cut/copy/paste directly under left hand
      _______, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), _______, _______,      KC_SLEP, KC_MINS       , KC_UNDS      , _______, _______ , _______, _______ ,
      _______,    _______,    _______,    _______,    _______,                                                 _______      , _______, _______ , _______, _______ ,

                                                               _______, _______,      _______, _______       ,
                                                                        _______,      _______,
  // Right thumb: Enter + word-delete (Ctrl+Bksp Win → Kanata Opt+Bksp Mac)
                                                      _______, _______, _______,      _______, LCTL(KC_ENTER), LCTL(KC_BSPC)
),

/*
 * SYMBOLS — hold C or comma on base
 * Bracket pairs sit on the same physical keys as C/V/D/F/comma/period so each finger learns one slot.
 * Row order: {}, (), [], <> — outside-in on the home-row-adjacent keys.
 */
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

/*
 * MEDIA — hold Esc on base (LT(MEDIA,KC_ESC))
 * Playback on left (Q/W/E area); volume on home row; F-keys across top for app-specific media binds.
 */
[MEDIA] = LAYOUT_ergodox_pretty(
LCTL(KC_GRV),   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,             KC_F11,      KC_F12 , KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , TO(BASE),
     _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, _______,            _______,      _______, _______, _______, _______, _______, _______, _______ ,
     _______, _______, KC_VOLD, KC_VOLU, _______, _______,                                   _______, _______, _______, _______, _______, _______ ,
     _______, _______, _______, _______, _______, _______,            _______,      _______, _______, _______, _______, _______, _______, _______ ,
     _______, _______, _______, _______, _______,                                                     _______, _______, _______, _______, _______ ,

  // Thumb: plain RGB + layer-color toggle — visual feedback without leaving MEDIA
                                                  _______,     RGB_MODE_PLAIN,      _______, _______,
                                                           TOGGLE_LAYER_COLOR,      _______,
                                         _______, _______,            _______,      _______, _______, _______
),

/*
 * NUMBERS — toggle on base (TT(NUMBERS))
 * Left half = arrow keys (spreadsheet/vim-style); right half = numpad under right-hand home area.
 * XXXXXXX blocks one thumb slot so NUMBERS doesn't accidentally stack with GAMING toggle.
 */
[NUMBERS] = LAYOUT_ergodox_pretty(
_______, _______, _______, _______,  _______, _______, _______,      _______, _______, _______, _______ , _______, _______, TO(BASE),
_______, _______, _______,   KC_UP,  _______, _______, _______,      _______, _______, KC_7   , KC_8    , KC_9   , KC_ASTR, _______ ,
_______, _______, KC_LEFT, KC_DOWN, KC_RIGHT, _______,                        _______, KC_4   , KC_5    , KC_6   , KC_PLUS, _______ ,
_______, _______, _______, _______,  _______, _______, _______,      _______, _______, KC_1   , KC_2    , KC_3   , KC_BSLS, _______ ,
_______, _______, _______, _______,  _______,                                          KC_0   , KC_COMMA, KC_DOT , KC_EQL , _______ ,

                                              _______, _______,      XXXXXXX, _______,
                                                       _______,      _______,
                                     _______, _______, _______,      _______, _______, _______
),

/*
 * MOUSE — toggle on base (TT(MOUSE))
 * ESDF cluster = pointer move (matches gaming WASD muscle memory); speed presets on top row.
 * QK_BOOT top-right: reflash entry without a dedicated boot key on base.
 */
[MOUSE] = LAYOUT_ergodox_pretty(
_______, MS_ACL0, MS_ACL1, MS_ACL2, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______,
_______, _______, MS_WHLU,   MS_UP, MS_WHLD, _______, _______,      _______, _______, _______, _______, _______, _______, QK_BOOT,
_______, MS_WHLL, MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLL,                        _______, _______, _______, _______, _______, _______,
  // Btn4/5 on outer columns = browser back/forward without leaving the layer
MS_BTN4, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, MS_BTN5,
_______, _______, _______, _______, MS_BTN1,                                          _______, _______, _______, _______, _______,

                                             _______, _______,      _______, _______,
                                                      _______,      _______,
                                    MS_BTN2, MS_BTN3, _______,      _______, _______, _______
),

/*
 * GAMING — toggle on base (TG(GAMING))
 * Strips home-row mods so games get clean key-down events; WASD + space on left, arrows on right.
 * LT(SHORTCUTS,Space) keeps chat/copy shortcuts reachable without leaving the layer.
 */
[GAMING] = LAYOUT_ergodox_pretty(
KC_TILDE, _______, _______, _______,  _______, _______,     _______,      _______, _______, _______, _______, _______, _______     , TO(BASE),
 _______, _______, _______, _______,  _______, _______,     _______,      _______, _______, _______, _______, _______, _______     , _______ ,
 LT(MEDIA,KC_ESC),    KC_A,    KC_S,    KC_D,     KC_F, _______,                            _______, KC_J   , KC_K   , KC_L   , KC_SEMICOLON, _______ ,
 KC_LSFT, _______, _______, _______,  _______, _______, MO(SYMBOLS),      _______, _______, _______, _______, _______, _______     , KC_RSFT ,
  // Bottom-left modifiers in standard gaming positions; space on thumb for jump
 KC_LCTL, KC_LALT, KC_LWIN, XXXXXXX, KC_SPACE,                                              _______, KC_LEFT, KC_UP  , KC_DOWN     , KC_RIGHT,

                                               _______,     _______,      _______, XXXXXXX,
                                               KC_VOLU,      _______,
                                      LT(SHORTCUTS,KC_SPACE), _______, KC_VOLD,      _______, _______, _______
)

#else // KANATA_MODE — Mode A: duplicate BASE / SHORTCUTS / GAMING per OS -----

/*
 * BASE_MAC — Mac default layer (selected by OS detect or manual toggle)
 * Same physical layout as BASE_WIN; only OS-specific modifiers and layer targets differ.
 * A/; use LCMD_T because Cmd is the primary Mac shortcut modifier (vs Ctrl on Windows).
 */
[BASE_MAC] = LAYOUT_ergodox_pretty(
         KC_GRV,         KC_1,         KC_2,             KC_3,          KC_4,           KC_5,          KC_MINUS,     KC_EQUAL            , KC_6       , KC_7        , KC_8                , KC_9        , KC_0                 , KC_EQUAL         ,
            KC_TAB,         KC_Q,         KC_W,             KC_E,          KC_R,           KC_T,          KC_LBRC,      KC_RBRC             , KC_Y       , KC_U        , KC_I                , KC_O        , KC_P                 , TD(TD_PIPE)      ,
  // LCMD_T on A/; for Cmd shortcuts; D/K stay LCTL_T for terminal/emacs-style bindings
  LT(MEDIA,KC_ESC), LCMD_T(KC_A), LALT_T(KC_S),     LCTL_T(KC_D),  LSFT_T(KC_F),           KC_G,                                              KC_H       , RSFT_T(KC_J), LCTL_T(KC_K)        , LALT_T(KC_L), LCMD_T(KC_SEMICOLON) , KC_QUOTE         ,
 TD(TD_CAPS_BASIC),     MY_MEH_Z,   MY_HYPER_X, LT(SYMBOLS,KC_C),          KC_V,           KC_B,   LCTL(KC_SPACE),      KC_TILDE             , KC_N       , KC_M        , LT(SYMBOLS,KC_COMMA), MY_HYPER_DOT, MT(MOD_RCTL,KC_SLASH), TD(TD_CAPS_BASIC),
TOGGLE_LAYER_COLOR,      _______,      _______,          _______, MO(SHORTCUTS_MAC),                                                                           TT(MOUSE)   , KC_LEFT             , KC_UP     , KC_DOWN              , KC_RIGHT          ,

                                                                                 LALT(KC_SPACE), LCTL(LSFT(KC_1)),      TG(GAMING_MAC)      , TT(NUMBERS),
                                                                                                 LCTL(LSFT(KC_2)),      _______             ,
  // Cmd+Ctrl+Space = emoji/special-char picker on Mac; Delete (not Bksp) = forward delete on Mac keyboards
                                                                       KC_SPACE,      KC_DELETE, LCTL(LSFT(KC_5)),      LCMD(LCTL(KC_SPACE)), KC_ENTER   , KC_BSPC
),

/*
 * BASE_WIN — Windows default layer
 * Mirror of BASE_MAC: LCTL_T on A/; for Ctrl shortcuts; Win+S on thumb for system search.
 */
[BASE_WIN] = LAYOUT_ergodox_pretty(
         KC_GRV,         KC_1,         KC_2,             KC_3,          KC_4,           KC_5,          KC_MINUS,     KC_EQUAL            , KC_6       , KC_7        , KC_8                , KC_9        , KC_0                 , KC_EQUAL         ,
            KC_TAB,         KC_Q,         KC_W,             KC_E,          KC_R,           KC_T,          KC_LBRC,      KC_RBRC             , KC_Y       , KC_U        , KC_I                , KC_O        , KC_P                 , TD(TD_PIPE)      ,
  LT(MEDIA,KC_ESC), LCTL_T(KC_A), LALT_T(KC_S),     LCTL_T(KC_D),  LSFT_T(KC_F),           KC_G,                                              KC_H       , RSFT_T(KC_J), LCTL_T(KC_K)        , LALT_T(KC_L), LCTL_T(KC_SEMICOLON) , KC_QUOTE         ,
 TD(TD_CAPS_BASIC),     MY_MEH_Z,   MY_HYPER_X, LT(SYMBOLS,KC_C),          KC_V,           KC_B,   LCTL(KC_SPACE),      KC_TILDE             , KC_N       , KC_M        , LT(SYMBOLS,KC_COMMA), MY_HYPER_DOT, MT(MOD_RCTL,KC_SLASH), TD(TD_CAPS_BASIC),
TOGGLE_LAYER_COLOR,      _______,      _______,          _______, MO(SHORTCUTS_WIN),                                                                           TT(MOUSE)   , KC_LEFT             , KC_UP     , KC_DOWN              , KC_RIGHT          ,

                                                                                 LALT(KC_SPACE), LCTL(LSFT(KC_1)),      TG(GAMING_WIN)      , TT(NUMBERS),
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
 LGUI(KC_GRV),    _______,    _______,    _______,    _______, _______,                        _______       , KC_LEFT      , KC_DOWN, KC_RIGHT, _______, KC_GRAVE,
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
 * MOUSE — same as Mode B, plus MY_OS_TOGGLE on left thumb
 * Infrequent layer = safe place for OS override (tap flip Mac/Win, hold re-detect).
 * See CROSS_PLATFORM.md for USB switcher fallback behavior.
 */
[MOUSE] = LAYOUT_ergodox_pretty(
_______, MS_ACL0, MS_ACL1, MS_ACL2, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______,
_______, _______, MS_WHLU,   MS_UP, MS_WHLD, _______, _______,      _______, _______, _______, _______, _______, _______, QK_BOOT,
_______, MS_WHLL, MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLL,                        _______, _______, _______, _______, _______, _______,
MS_BTN4, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, MS_BTN5,
_______, _______, _______, _______, MS_BTN1,                                          _______, _______, _______, _______, _______,

                                             _______, _______,      _______, _______,
                                                      _______,      _______,
                                    MS_BTN2, MS_BTN3, MY_OS_TOGGLE,      _______, _______, _______
),

// GAMING_* differ only in TO() / LT(SHORTCUTS_*) targets — bottom row uses native OS GUI/Win keys
[GAMING_MAC] = LAYOUT_ergodox_pretty(
KC_TILDE, _______, _______, _______,  _______, _______,     _______,      _______, _______, _______, _______, _______, _______     , TO(BASE_MAC),
 _______, _______, _______, _______,  _______, _______,     _______,      _______, _______, _______, _______, _______, _______     , _______ ,
 LT(MEDIA,KC_ESC),    KC_A,    KC_S,    KC_D,     KC_F, _______,                            _______, KC_J   , KC_K   , KC_L   , KC_SEMICOLON, _______ ,
 KC_LSFT, _______, _______, _______,  _______, _______, MO(SYMBOLS),      _______, _______, _______, _______, _______, _______     , KC_RSFT ,
 KC_LCTL, KC_LALT, KC_LGUI, XXXXXXX, KC_SPACE,                                              _______, KC_LEFT, KC_UP  , KC_DOWN     , KC_RIGHT,

                                               _______,     _______,      _______, XXXXXXX,
                                               KC_VOLU,      _______,
                                      LT(SHORTCUTS_MAC,KC_SPACE), _______, KC_VOLD,      _______, _______, _______
),

[GAMING_WIN] = LAYOUT_ergodox_pretty(
KC_TILDE, _______, _______, _______,  _______, _______,     _______,      _______, _______, _______, _______, _______, _______     , TO(BASE_WIN),
 _______, _______, _______, _______,  _______, _______,     _______,      _______, _______, _______, _______, _______, _______     , _______ ,
 LT(MEDIA,KC_ESC),    KC_A,    KC_S,    KC_D,     KC_F, _______,                            _______, KC_J   , KC_K   , KC_L   , KC_SEMICOLON, _______ ,
 KC_LSFT, _______, _______, _______,  _______, _______, MO(SYMBOLS),      _______, _______, _______, _______, _______, _______     , KC_RSFT ,
 KC_LCTL, KC_LALT, KC_LWIN, XXXXXXX, KC_SPACE,                                              _______, KC_LEFT, KC_UP  , KC_DOWN     , KC_RIGHT,

                                               _______,     _______,      _______, XXXXXXX,
                                               KC_VOLU,      _______,
                                      LT(SHORTCUTS_WIN,KC_SPACE), _______, KC_VOLD,      _______, _______, _______
)

#endif // KANATA_MODE
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
#ifndef KANATA_MODE
    // Restore manual OS lock from EEPROM, else wait for / run auto-detect
    load_user_config();
    if (user_config.manual_lock) {
        manual_os_locked = true;
        apply_os_layer(user_config.os_is_mac);
    } else {
        apply_os_from_detection();
    }
#else
    rgb_matrix_set_color_all(97, 0, 255); // Mode B boot color (purple)
#endif
}

// Per-layer RGB helpers — LED indices from key_indexes.h
static void rgb_indicators_base(void) {
    rgb_matrix_set_color_all(97, 0, 255);
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
#ifndef KANATA_MODE
    // White accent on thumb cluster when manual OS lock is active
    if (manual_os_locked) {
        rgb_matrix_set_color(IDX_R2, 255, 255, 255);
        rgb_matrix_set_color(IDX_R3, 255, 255, 255);
    }
#endif
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

#ifndef KANATA_MODE
    // Suppress layer colors briefly after OS detect/toggle flash
    if (os_rgb_active) {
        return true;
    }
#endif

    switch (get_highest_layer(layer_state)) {
#ifdef KANATA_MODE
        case BASE:
#else
        case BASE_MAC:
        case BASE_WIN:
#endif
            rgb_indicators_base();
            break;

#ifdef KANATA_MODE
        case SHORTCUTS:
#else
        case SHORTCUTS_MAC:
        case SHORTCUTS_WIN:
#endif
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
#ifndef KANATA_MODE
            // OS toggle key: white = Mac mode, blue = Windows mode
            rgb_matrix_set_color(IDX_R2, os_is_mac ? 255 : 0, os_is_mac ? 255 : 0, os_is_mac ? 255 : 255);
#endif
            break;

#ifdef KANATA_MODE
        case GAMING:
#else
        case GAMING_MAC:
        case GAMING_WIN:
#endif
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

        // Continue Caps Word without shifting (underscore is on TD_DASH double-tap).
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
    switch (keycode) {
        case RGB_SLD:
            if (record->event.pressed)
                rgblight_mode(1);

            return false;
        case KC_ESC:
            if (get_oneshot_mods() != 0)
                clear_oneshot_mods();

            break;
        case SUPER_ALT_TAB:
            // App switcher: register Cmd or Ctrl once, then Tab on each press; release in matrix_scan_user
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
#ifndef KANATA_MODE
                    register_code(is_mac_os() ? KC_LGUI : KC_LEFT_CTRL);
#else
                    register_code(KC_LEFT_CTRL); // Kanata maps to Cmd on Mac
#endif
                }

                alt_tab_timer = timer_read();
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }

            return false;
#ifndef KANATA_MODE
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
#endif
        case COLON_EQUAL:
            if (record->event.pressed)
                SEND_STRING(":=");

            break;
    }
    return true;
}

void matrix_scan_user(void) {
#ifndef KANATA_MODE
    // End full-keyboard OS flash after ~300 ms
    if (os_rgb_active && timer_elapsed(os_rgb_timer) > 300) {
        os_rgb_active = false;
    }
#endif

    // Release Cmd/Ctrl after SUPER_ALT_TAB idle timeout
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 750) {
#ifndef KANATA_MODE
            unregister_code(is_mac_os() ? KC_LGUI : KC_LEFT_CTRL);
#else
            unregister_code(KC_LEFT_CTRL);
#endif
            is_alt_tab_active = false;
        }
    }
}

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
