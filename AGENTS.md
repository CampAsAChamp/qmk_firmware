# QMK firmware (CampAsAChamp fork)

Personal fork of [QMK](https://github.com/qmk/qmk_firmware) tracking upstream. Primary work: maintain the `campasachamp` keymaps.

- `origin` — CampAsAChamp/qmk_firmware
- `upstream` — qmk/qmk_firmware

## Editable scope

**Only modify files under these keymap directories unless explicitly asked:**

| Keyboard | Path |
|----------|------|
| ErgoDox EZ Glow (primary) | `keyboards/ergodox_ez/glow/keymaps/campasachamp/` |
| DZ60RGB ANSI | `keyboards/dztech/dz60rgb_ansi/keymaps/campasachamp/` |

Do **not** change `quantum/`, `tmk_core/`, other keyboards, or `lib/` submodule pins unless the user requests it.

Submodule drift after `git pull`: reset with `git submodule update --init --recursive` (see `.cursor/rules/qmk-git-submodules.mdc`).

## Build and verify

Repo shortcut: `qmkfm` (do **not** use `qmk` as a zsh named directory — it shadows the QMK CLI).

| Action | Command |
|--------|---------|
| Compile ErgoDox | `qce` |
| Flash ErgoDox | `qfe` (only when user asks) |
| Compile DZ60 | `qcd` |
| Flash DZ60 | `qfd` (only when user asks) |

After keymap changes, compile the affected keyboard. Toolchain and Homebrew setup: `.cursor/rules/qmk-setup.mdc`.

## ErgoDox keymap (primary)

Self-contained in `keyboards/ergodox_ez/glow/keymaps/campasachamp/`.

| File | Purpose |
|------|---------|
| `keymap.c` | Layers, tap dances, combos, RGB indicators, `process_record_user` |
| `config.h` | `TAPPING_TERM 200`, `PERMISSIVE_HOLD`, RGB animation disables (firmware space) |
| `rules.mk` | Feature flags: `TAP_DANCE_ENABLE`, `COMBO_ENABLE`, `LEADER_ENABLE`, `CAPS_WORD_ENABLE`, `RGB_MATRIX_ENABLE` |
| `key_indexes.h` | RGB LED index constants (`IDX_Q`, `IDX_L4`, etc.) |

**Layers:** `BASE`, `SHORTCUTS`, `SYMBOLS`, `MEDIA`, `NUMBERS`, `MOUSE`, `GAMING`

**Patterns:**
- Home-row mods: `LCMD_T`, `LALT_T`, `LCTL_T`, `LSFT_T` / right-hand equivalents
- `MY_HYPER`, `MY_MEH`, mod-tap variants on Z/X/period
- Tap dances: `TD_DASH`, bracket dances, caps (`TD_CAPS_BASIC` / `TD_CAPS_ADVANCED`)
- Combo: `G` + `H` → Hyper+Enter
- Custom keycodes: `SUPER_ALT_TAB`, `COLON_EQUAL`, `RGB_SLD`
- Per-layer RGB in `rgb_matrix_indicators_user` — use `IDX_*` from `key_indexes.h`, not raw numbers
- Blank layer template at bottom of `keymap.c` for new layers

## DZ60 keymap (secondary)

`keyboards/dztech/dz60rgb_ansi/keymaps/campasachamp/` — similar features, different layer names:

`BASE_LAYER`, `SHORTCUT_LAYER`, `SYMBOL_LAYER`, `MEDIA_LAYER`, `RGB_LAYER`, `MOUSE_LAYER`

Keep naming consistent within each board; boards do not need matching layer enums.

## Conventions

- Minimize diff scope; match existing style in the file being edited
- Plain imperative commit messages (no conventional commits), e.g. `Add gaming layer shortcuts`
- Do not commit unless asked
- Do not flash unless asked
