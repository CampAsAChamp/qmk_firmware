# campasachamp — ErgoDox EZ Glow

Personal QMK keymap for the ErgoDox EZ Glow. Home-row mods, layered shortcuts, per-layer RGB, and cross-platform Mac/Windows support via firmware OS detection.

**Keyboard:** `ergodox_ez/glow`  
**Keymap:** `campasachamp`

---

## Build and flash

```bash
qce          # compile
qfe          # flash when ready
```

From the repo root you can also run:

```bash
qmk compile -kb ergodox_ez/glow -km campasachamp
qmk flash -kb ergodox_ez/glow -km campasachamp
```

---

## Layers

| Layer | How to reach it | Purpose |
|-------|-----------------|---------|
| **BASE** | Default | QWERTY, home-row mods, layer keys, thumb cluster |
| **SHORTCUTS** | Hold `MO(SHORTCUTS)` on base (left bottom row) | Copy/paste, window/tab chords, word-delete, nav arrows |
| **SYMBOLS** | Hold **C** or **comma** on base | Bracket pairs `{}` `()` `[]` `<>` on familiar finger positions |
| **MEDIA** | Hold **Esc** on base (`LT(MEDIA,KC_ESC)`) | Play/pause, volume, F-keys, RGB controls |
| **NUMBERS** | Toggle thumb key (`TT(NUMBERS)`) | Numpad on right hand; arrow keys on left |
| **MOUSE** | Toggle thumb key (`TT(MOUSE)`) on base | Pointer, scroll, mouse buttons; OS toggle |
| **GAMING** | Toggle thumb key (`TG(GAMING)`) | Plain WASD layout without home-row mods |

BASE / SHORTCUTS / GAMING exist as `_MAC` and `_WIN` pairs so shortcuts use the correct OS modifiers. SYMBOLS, MEDIA, NUMBERS, and MOUSE are shared.

---

## BASE layer

### Home-row mods

Tapping term: **200 ms** with **permissive hold** (`config.h`).

| Key | Tap | Hold (Mac) | Hold (Win) |
|-----|-----|------------|------------|
| **A** / **;** | Letter | Ctrl | Win |
| **S** | Letter | Alt | Ctrl |
| **L** | Letter | Alt | Alt |
| **D** / **K** | Letter | Cmd | Ctrl |
| **F** / **J** | Letter | Shift | Shift |
| **Esc** (left pinky) | Esc | MEDIA layer | MEDIA layer |

Mac and Windows use the **opposite** mapping on the same keys: Mac has Ctrl on A/; and Cmd on D/K; Windows has Win on A/; and Ctrl on D/K.

### Modifier keys on bottom row

| Key | Tap | Hold |
|-----|-----|------|
| **Z** | z | Meh (Shift+Ctrl+Alt) |
| **X** | x | Hyper (Shift+Ctrl+Alt+Cmd) |
| **.** | . | Hyper |

Meh/Hyper sit on Z/X/period because those letters rarely conflict with double-tap typing.

### Other base-layer highlights

- **`[` / `]`** under **T** / **Y** — quick bracket access while coding
- **`TD(TD_PIPE)`** on **P** — tap `\`, double-tap `\|` (pipe)
- **`TD(TD_CAPS_BASIC)`** on outer pinkies — 1× shift, 2× caps word, 3× caps lock
- **`LCTL(KC_SPACE)`** on **B** — Spotlight (Mac) / search-style chord
- **Arrow cluster** on right bottom row — one-handed navigation
- **Thumb cluster** — Space, Delete, Enter, Backspace, IDE chords (`Ctrl+Shift+1/2/5`), gaming/numbers toggles

Mac thumb extras: **Cmd+Ctrl+Space** (emoji/special character picker).  
Windows thumb: **Win+S** (system search).

---

## SHORTCUTS layer

Hold the **SHORTCUTS** key on the base layer (left side, bottom row).

| Area | Keys | Actions |
|------|------|---------|
| Left, QWERTY-aligned | Q W R T | Close tab, close window, reload, new tab |
| Left, home row | Z X C V | Undo, cut, copy, paste |
| Right | I J K L + arrows | Home, arrows, End |
| Top | F1–F12 | Function keys |
| Thumb (right) | | **Word-delete** + Enter |

**Word-delete:** hold SHORTCUTS and press the right-thumb key — Option+Backspace on Mac, Ctrl+Backspace on Windows.

**App switcher:** **SUPER_ALT_TAB** sends Cmd+Tab (Mac) or Ctrl+Tab (Win) with hold-to-repeat Tab behavior.

---

## SYMBOLS layer

Hold **C** or **comma** on base. Bracket pairs mirror the physical keys you already use for those letters:

| Physical key (base) | Symbol pair |
|--------------------|-------------|
| E / R row (via layer) | `{` `}` |
| D / F row | `(` `)` |
| C / V row | `[` `]` |
| Comma / period row | `<` `>` |

Tap-dance bracket keys on outer columns also provide `( ) { } [ ] < >` via multi-tap.

---

## MEDIA layer

Hold **Esc** on base.

- **Q W E** — previous, play/pause, next track
- **S row** — volume down / up
- **F1–F12** across the top
- Thumb — RGB plain mode and layer-color toggle

---

## NUMBERS layer

Toggle from base thumb cluster (`TT(NUMBERS)`).

- **Left half** — arrow keys (spreadsheet / nav)
- **Right half** — numpad under **7 8 9**, **4 5 6**, **1 2 3**, **0** and operators

---

## MOUSE layer

Toggle from base (`TT(MOUSE)`).

- **ESDF cluster** — pointer move (same muscle memory as WASD)
- **Top row** — mouse acceleration presets
- **Outer columns** — mouse buttons 4/5 (e.g. browser back/forward)
- **Top-right** — `QK_BOOT` (enter bootloader to reflash)
- **Left thumb (Mode A only)** — **MY_OS_TOGGLE** — manual Mac/Win override (see [CROSS_PLATFORM.md](CROSS_PLATFORM.md))

---

## GAMING layer

Toggle from base (`TG(GAMING)`). Strips home-row mods for reliable key-down in games.

- **WASD** on left, **JKL;** on right
- **Space** on left thumb; **LT(SHORTCUTS, Space)** keeps chat/copy shortcuts reachable
- Bottom-left — Shift, Ctrl, Alt, GUI/Win in standard gaming positions
- Right side — arrow keys

---

## Tap dances

| Key | Taps | Output |
|-----|------|--------|
| **TD_DASH** | 1 / 2 | `-` / `_` |
| **TD_PLUS** | 1 / 2 | `=` / `+` |
| **TD_LBRACKET** | 1–4 | `(`, `{`, `[`, `<` |
| **TD_RBRACKET** | 1–4 | `)`, `}`, `]`, `>` |
| **TD_PIPE** (P) | 1 / 2 | `\` / `\|` |
| **TD_CAPS_BASIC** | 1 / 2 / 3 | one-shot shift / caps word / caps lock |

Bracket dances live on punctuation keys to avoid interfering with normal letter typing.

---

## Combos

| Combo | Output | Why |
|-------|--------|-----|
| **G + H** (press together) | Hyper + Enter | Quick “submit” from index-finger home row |

---

## Custom keycodes

| Keycode | Where | Behavior |
|---------|-------|----------|
| **SUPER_ALT_TAB** | SHORTCUTS | App switcher (Cmd+Tab / Ctrl+Tab) |
| **COLON_EQUAL** | (available in keymap) | Types `:=` |
| **MY_OS_TOGGLE** | MOUSE (Mode A) | Tap: lock Mac↔Win; hold: unlock + re-detect |
| **RGB_SLD** | — | RGB solid mode |

---

## RGB indicators

Per-layer colors in `rgb_matrix_indicators_user` (LED indices in `key_indexes.h`):

| Layer | Visual |
|-------|--------|
| **BASE** | Mac: purple base; Win: blue base; green on symbol keys (Z X C , . /); orange on G H; red on layer keys |
| **SHORTCUTS** | Red F-row; blue shortcut keys; white/orange/purple accents on nav/edit keys |
| **SYMBOLS** | Red E/R, white D/F, blue C/V |
| **MEDIA** | Blue F-row; green transport; purple volume keys |
| **NUMBERS** | Orange numpad keys |
| **MOUSE** | White pointer cluster; blue movement keys |
| **GAMING** | Blue fill; white gaming layer indicator |

**OS feedback:**

- Full-keyboard flash: **white** = Mac, **blue** = Windows (~300 ms on detect/toggle)
- Manual OS lock: white accent on base thumb cluster
- MOUSE layer OS toggle key: white (Mac) or blue (Win)

RGB animations are mostly disabled in `config.h` to save firmware space.

---

## Cross-platform (Mac + Windows)

This keymap is used across a MacBook (via USB-C dock) and a Windows PC through a USB switcher. QMK auto-detects the host OS and selects paired `_MAC` / `_WIN` layers; a manual toggle on the MOUSE layer covers USB-switcher edge cases.

**Before flashing:** reset ErgoDox modifier keys to defaults in macOS System Settings (do not use the per-device Ctrl↔Cmd swap).

Full setup, testing, and USB switcher notes: **[CROSS_PLATFORM.md](CROSS_PLATFORM.md)**

Archived Kanata-based alternative (Mode B): git branch **`kanata-mode`** in this repo.

---

## QMK features enabled

From `rules.mk`:

- Tap dance, combos, leader, caps word
- RGB matrix (per-key LEDs)
- Mouse keys, NKRO
- OS detection

From `config.h`:

- `TAPPING_TERM 200`, `PERMISSIVE_HOLD`
- Leader with no timeout
- RGB timeout 15 minutes
- Mouse wheel acceleration tuned for quick scrolling

---

## Files

| File | Purpose |
|------|---------|
| `keymap.c` | Layers, tap dances, combos, OS detect, RGB, custom keycodes |
| `config.h` | Tapping term, OS detection flags, RGB/mouse tuning |
| `rules.mk` | Feature flags |
| `key_indexes.h` | RGB LED index constants (`IDX_Q`, `IDX_L4`, …) |
| `README.md` | This document |
| `CROSS_PLATFORM.md` | Mac/Windows setup, migration, testing |

---

## Related keymaps

A similar **campasachamp** keymap exists for the DZ60RGB ANSI (`keyboards/dztech/dz60rgb_ansi/keymaps/campasachamp/`). Layer names differ slightly; ErgoDox is the primary board for cross-platform work.
