# Cross-platform ErgoDox (campasachamp)

Mac and Windows use different modifiers for the same actions. This keymap handles that in firmware with paired `_MAC` / `_WIN` layers, OS auto-detect, and a manual toggle on the MOUSE layer.

**Do not** re-enable the macOS System Settings Ctrl↔Cmd swap for the ErgoDox — that double-remaps with firmware layers.

Archived Kanata-based alternative (former Mode B): git branch **`kanata-mode`** in this repo.

---

## Step 0 — Retire macOS ErgoDox modifier swap

Before flashing:

1. **System Settings → Keyboard → Modifier Keys → ErgoDox EZ Glow**
2. Reset all keys to defaults (Control→Control, Command→Command, etc.)
3. Do not re-enable the swap

Internal MacBook keyboard stays at defaults. If you use Kanata on the MacBook, keep it scoped to the internal keyboard only — do not remap the ErgoDox there.

---

## Firmware approach

### What it does

- **Paired layers:** `BASE_MAC` / `BASE_WIN`, `SHORTCUTS_MAC` / `SHORTCUTS_WIN`
- **Shared layers:** SYMBOLS, MEDIA, NUMBERS, MOUSE
- **Gaming:** `GAMING` layer only (toggle from `BASE_WIN` thumb cluster)
- **Auto-detect** on USB connect / switch (with USB-switcher reset flags in `config.h`)
- **Manual toggle** on MOUSE layer if auto-detect fails

### Build and flash

```bash
qce
# or: qmk compile -kb ergodox_ez/glow -km campasachamp
qfe   # when ready to flash
```

### OS auto-detect

On plug-in or USB switch (if the switch re-enumerates), firmware selects Mac or Windows layers within ~1 second.

RGB flash on detect/toggle:

- **White** = Mac mode
- **Blue** = Windows mode

### Manual OS toggle (MOUSE layer)

Hold **MOUSE** (`TT(MOUSE)` on base), press the thumb key mapped to **MY_OS_TOGGLE** (left thumb cluster).

| Action | Result |
|---|---|
| **Tap** | Flip Mac ↔ Win; lock manual mode; save to EEPROM |
| **Hold** (>200 ms) | Clear manual lock; re-run auto-detect |

MOUSE layer RGB on the OS toggle key: **white** = Mac locked, **blue** = Windows locked.

After toggling OS, firmware returns to the base layer for RGB feedback (purple Mac / blue Win) even if the MOUSE access key is still held; release that key to use MOUSE again.

---

## Logical actions reference

| Logical action | macOS | Windows | ErgoDox location |
|---|---|---|---|
| Word navigation | Option + Left/Right | Ctrl + Left/Right | Hold **S** on BASE + arrow keys (S = Alt on Mac, Ctrl on Win) |
| Word delete | Option + Backspace | Ctrl + Backspace | Hold SHORTCUTS + thumb key (right cluster) |
| Copy | Cmd + C | Ctrl + C | SHORTCUTS — C key |
| Paste | Cmd + V | Ctrl + V | SHORTCUTS — V key |
| Undo | Cmd + Z | Ctrl + Z | SHORTCUTS — Z key |
| Cut | Cmd + X | Ctrl + X | SHORTCUTS — X key |
| Close tab | Cmd + W | Ctrl + W | SHORTCUTS — W key |
| New tab | Cmd + T | Ctrl + T | SHORTCUTS — T key |
| App switcher | Cmd + Tab | Ctrl + Tab | SHORTCUTS — SUPER_ALT_TAB |
| Home-row outer mod | Ctrl (hold A/;) | Win (hold A/;) | A and ; mod-tap on BASE |
| Home-row inner mod | Cmd (hold D/K) | Ctrl (hold D/K) | D and K mod-tap on BASE |
| Spotlight / search | Cmd+Ctrl+Space | Win + S | BASE thumb cluster |

Firmware implements these in paired `_MAC` / `_WIN` layers.

---

## USB switcher

4-port switch: PC on one output, MacBook dock on the other.

- If the switch re-enumerates USB, auto-detect should pick the right OS layer.
- If shortcuts feel wrong after switching, hold MOUSE and tap **MY_OS_TOGGLE**.
- Hold **MY_OS_TOGGLE** to clear manual lock and re-sync to auto-detect.

---

## Test checklist

- [ ] USB switch PC → Mac: Mac layers within ~1 s, or MOUSE toggle fixes it
- [ ] SHORTCUTS + thumb key: word-delete on both OSes
- [ ] Home-row Mac: hold A → Ctrl, hold D → Cmd
- [ ] Home-row Win: hold A → Win, hold D → Ctrl
- [ ] Long-press OS toggle: clears manual lock, re-syncs
- [ ] Unplug/replug with manual lock: EEPROM restores last mode

---

## Pitfalls

- Re-enabling macOS ErgoDox modifier swap → double remapping
- Kanata remapping the ErgoDox while firmware layers are active → double remapping
- Flash usage is high — avoid adding large new QMK features without disabling something else

---

## Files

| File | Purpose |
|---|---|
| `README.md` | Keymap overview — layers, mods, tap dances, RGB, build commands |
| `keymap.c` | Layers, OS detect, manual toggle, RGB |
| `config.h` | Tapping term, OS detection flags, EEPROM size |
| `rules.mk` | Feature flags |
| `CROSS_PLATFORM.md` | This document |
