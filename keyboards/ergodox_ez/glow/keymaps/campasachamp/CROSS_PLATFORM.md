# Cross-platform ErgoDox (campasachamp)

Mac and Windows use different modifiers for the same actions. This keymap supports two test modes — pick **one** for the ErgoDox at a time.

| | Mode A — Firmware (default) | Mode B — Kanata |
|---|---|---|
| Flash | `qce` | `KANATA_MODE=yes qce` |
| OS switching | QMK auto-detect + MOUSE-layer toggle | Kanata `(mac?)` / `(win?)` on active host |
| Kanata | Running, **excludes** ErgoDox | Running, **includes** ErgoDox |
| macOS ErgoDox modifier swap | **Off** | **Off** |

**Never** run Mode A firmware + Mode B Kanata remapping on the ErgoDox at the same time. **Never** re-enable the macOS System Settings Ctrl↔Cmd swap for the ErgoDox.

---

## Step 0 — Retire macOS ErgoDox modifier swap

Before flashing:

1. **System Settings → Keyboard → Modifier Keys → ErgoDox EZ Glow**
2. Reset all keys to defaults (Control→Control, Command→Command, etc.)
3. Do not re-enable the swap

Internal MacBook keyboard stays at defaults; Kanata continues handling it separately.

---

## Mode A — Firmware (default)

### What it does

- **Paired layers:** `BASE_MAC` / `BASE_WIN`, `SHORTCUTS_MAC` / `SHORTCUTS_WIN`, `GAMING_MAC` / `GAMING_WIN`
- **Shared layers:** SYMBOLS, MEDIA, NUMBERS, MOUSE
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

### Kanata setup (Mode A)

Kanata should **not** remap the ErgoDox. Keep your existing MacBook `defsrc` only. Example — exclude ErgoDox by name in your main config, or simply do not add ErgoDox-specific rules.

---

## Mode B — Kanata

### What it does

- QMK sends **Windows-convention** key output (Ctrl shortcuts, `LCTL_T` home-row mods)
- Kanata on the host applies Mac remapping via `(mac?)` rules
- USB switch follows active host OS immediately (no firmware re-detect)

### Build and flash

```bash
KANATA_MODE=yes qce
qfe
```

### Kanata config

See [kanata.kbd](kanata.kbd) in this directory. Merge into your live Kanata setup or `(include)` it.

**ErgoDox USB IDs:** VID `0x3297`, PID `0x4976` (ErgoDox EZ Glow)

Install Kanata on home PC if not already (work machine already has it):

```bash
# macOS (Homebrew)
brew install kanata

# Windows — see https://github.com/jtroo/kanata/releases
```

Run Kanata with your merged config at login (same as work).

---

## Logical actions reference

Keep Mode A firmware and Mode B Kanata aligned to this table:

| Logical action | macOS | Windows | ErgoDox location |
|---|---|---|---|
| Word delete | Option + Backspace | Ctrl + Backspace | Hold SHORTCUTS + thumb key (right cluster) |
| Copy | Cmd + C | Ctrl + C | SHORTCUTS — C key |
| Paste | Cmd + V | Ctrl + V | SHORTCUTS — V key |
| Undo | Cmd + Z | Ctrl + Z | SHORTCUTS — Z key |
| Cut | Cmd + X | Ctrl + X | SHORTCUTS — X key |
| Close tab | Cmd + W | Ctrl + W | SHORTCUTS — W key |
| New tab | Cmd + T | Ctrl + T | SHORTCUTS — T key |
| App switcher | Cmd + Tab | Ctrl + Tab | SHORTCUTS — SUPER_ALT_TAB |
| Home-row command mod | Cmd (hold) | Ctrl (hold) | A and ; mod-tap on BASE |
| Spotlight / search | Cmd+Ctrl+Space | Win + S | BASE thumb cluster |

Mode A implements these in paired `_MAC` / `_WIN` layers. Mode B sends Windows chords from QMK; Kanata maps them on Mac.

---

## USB switcher

4-port switch: PC on one output, MacBook dock on the other.

**Mode A:**

- If the switch re-enumerates USB, auto-detect should pick the right OS layer.
- If shortcuts feel wrong after switching, hold MOUSE and tap **MY_OS_TOGGLE**.
- Hold **MY_OS_TOGGLE** to clear manual lock and re-sync to auto-detect.

**Mode B:**

- Kanata on whichever machine is active handles OS conditionals — no firmware action needed.

---

## A/B testing procedure

1. Complete Step 0 (reset macOS modifier swap).
2. Flash **Mode A** (`qce`). Test for several days with Kanata excluding ErgoDox.
3. Flash **Mode B** (`KANATA_MODE=yes qce`). Enable ErgoDox rules in Kanata. Test with USB switching.
4. Pick a winner; flash that build long-term.

---

## Test checklist

### Mode A

- [ ] USB switch PC → Mac: Mac layers within ~1 s, or MOUSE toggle fixes it
- [ ] SHORTCUTS + thumb key: word-delete on both OSes
- [ ] Home-row mod-tap on A: Cmd on Mac, Ctrl on Windows
- [ ] Long-press OS toggle: clears manual lock, re-syncs
- [ ] Unplug/replug with manual lock: EEPROM restores last mode

### Mode B

- [ ] Kanata running on work PC and home PC
- [ ] USB switch: shortcuts follow host OS immediately
- [ ] Word-delete and copy/paste work on both OSes

---

## Pitfalls

- Re-enabling macOS ErgoDox modifier swap with Mode A or B → double remapping
- Mode A + Mode B both active on ErgoDox → double remapping
- Firmware at **97%** flash usage in Mode A — avoid adding large new QMK features without disabling something else

---

## Files

| File | Purpose |
|---|---|
| `README.md` | Keymap overview — layers, mods, tap dances, RGB, build commands |
| `keymap.c` | Layers, OS detect, manual toggle, RGB |
| `config.h` | Tapping term, OS detection flags, EEPROM size |
| `rules.mk` | Feature flags, `KANATA_MODE` toggle |
| `kanata.kbd` | Mode B Kanata reference config |
| `CROSS_PLATFORM.md` | This document |
