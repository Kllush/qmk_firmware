VIA_ENABLE = yes
LTO_ENABLE = yes

EXTRAFLAGS += -flto # Make the hex smaller
LINK_TIME_OPTIMIZATION_ENABLE = yes
MOUSEKEY_ENABLE = no	# Mouse keys(+4700)
CONSOLE_ENABLE = no	# Console for debug(+400)
COMMAND_ENABLE = no    # Commands for debug and configuration
UNICODE_ENABLE   = no  # Unicode
BACKLIGHT_ENABLE = no  # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = yes
ENCODER_ENABLE = yes
TAP_DANCE_ENABLE = yes
AUDIO_ENABLE = no
#RGB_MATRIX_ENABLE = yes
#RGB_MATRIX_DRIVER = WS2812
DYNAMIC_MACRO_ENABLE = yes
#NO_USB_STARTUP_CHECK = yes
SPACE_CADET_ENABLE = no
