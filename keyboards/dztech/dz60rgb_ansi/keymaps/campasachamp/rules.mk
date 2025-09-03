# # MCU name
# MCU = atmega32u4

# # Bootloader selection
# BOOTLOADER = atmel-dfu

# Build Options
#   change yes to no to disable
#
AUDIO_ENABLE = no              # Audio output
BACKLIGHT_ENABLE = no          # Enable keyboard backlight functionality
BOOTMAGIC_ENABLE = no     # Enable Bootmagic Lite
CAPS_WORD_ENABLE    = yes
COMMAND_ENABLE = no            # Commands for debug and configuration
CONSOLE_ENABLE = no            # Console for debug
EXTRAKEY_ENABLE = yes          # Audio control and System control
LEADER_ENABLE = yes # To add support for Leader Key you simply need to add a single line to your keymap’s rules.mk:
LTO_ENABLE = yes
MOUSEKEY_ENABLE = yes          # Mouse keys
NKRO_ENABLE = yes           # Enable N-Key Rollover
NO_USB_STARTUP_CHECK = no      # Disable initialization only when usb is plugged in
RGB_MATRIX_ENABLE = yes        # Use RGB matrix
RGBLIGHT_ENABLE = no           # Enable keyboard RGB underglow
TAP_DANCE_ENABLE = yes
