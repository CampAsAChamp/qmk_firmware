# If you have Left LEDs (see
# https://geekhack.org/index.php?topic=22780.msg873819#msg873819 for
# details), include the following define:
# OPT_DEFS += -DLEFT_LEDS

# Build Options
BOOTMAGIC_ENABLE 	= no   	         # Enable Bootmagic Lite
CAPS_WORD_ENABLE    = yes
COMMAND_ENABLE   	= no   	         # Commands for debug and configuration
CONSOLE_ENABLE   	= no   	         # Console for debug
CUSTOM_MATRIX    	= lite 	         # Custom matrix file for the ErgoDox EZ
DEBOUNCE_TYPE 		= sym_eager_pr
EXTRAKEY_ENABLE  	= yes  	         # Audio control and System control
LEADER_ENABLE       = yes
MOUSEKEY_ENABLE  	= yes  	         # Mouse keys
NKRO_ENABLE      	= yes  	         # Enable N-Key Rollover
# RGB_MATRIX_DRIVER 	= IS31FL3731
RGBLIGHT_ENABLE     = no             # Only available on the Shine
RGB_MATRIX_ENABLE   = yes
SWAP_HANDS_ENABLE	= no   	         # Allow swapping hands of keyboard
TAP_DANCE_ENABLE 	= yes
UNICODE_ENABLE   	= no   	         # Unicode
# Disable unsupported hardware
AUDIO_SUPPORTED = no
BACKLIGHT_SUPPORTED = no
MOUSE_SHARED_EP = no

# project specific files
SRC += matrix.c

I2C_DRIVER_REQUIRED = yes

# INTROSPECTION_KEYMAP_C = campasachamp.c	   

QUANTUM_LIB_SRC += i2c_master.c

DEFAULT_FOLDER = ergodox_ez/base
