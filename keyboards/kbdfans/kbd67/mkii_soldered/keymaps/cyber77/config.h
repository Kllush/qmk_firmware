// Backlight pin unused, will be repurposed into speaker pin later
#undef BACKLIGHT_PIN
#undef DEBOUNCE

/* Custom user specific features
*
*  Startup animation duration time and fade duration can be set here.
*
*  Defaults:
*  STARTUP_ANIM_TIME = 2200
*  FADE_TIME = 4    (Fast = 3 / Medium = 6 / Slow = 10)
*/
//#define STARTUP_ANIM_TIME 2200
//#define FADE_TIME 4

// Enable features
#define DYNAMIC_MACRO_NO_NESTING
#define TAPPING_TERM_PER_KEY

#define USB_POLLING_INTERVAL_MS 2
#define QMK_KEYS_PER_SCAN 4
#define DEBOUNCE 3

// Free up space
#define NO_ACTION_ONESHOT
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE

//Encoder pins
#ifdef ENCODER_ENABLE
    #define ENCODERS_PAD_A { B3 } // MISO
    #define ENCODERS_PAD_B { B1 } // SCLK / SCK
    #define ENCODER_DEFAULT_POS 0x3 // Fix encoder skips when changing direction
#endif

// RGB matrix
#ifdef RGB_MATRIX_ENABLE
    //RGB Matrix Config
    #define RGBW
    #define RGB_DI_PIN B2                       // Assign to pin B2 / MOSI on board
    #define DRIVER_LED_TOTAL 76                 // The number of LEDs connected
    #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 190
    #define RGB_MATRIX_STARTUP_SPD 64
    #define RGB_DISABLE_WHEN_USB_SUSPENDED
    #define RGB_CUSTOM_TIMEOUT_DELAY 600000     // Timout delay in ms. Set to 10 minutes

    //RGB Matrix Effects
    #define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CUSTOM_HUE_PENDULUM2
    #define RGB_MATRIX_STARTUP_HUE 170

    #define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
    #define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
    #define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
    #define ENABLE_RGB_MATRIX_CYCLE_SPIRAL
    #define ENABLE_RGB_MATRIX_RAINBOW_BEACON
    #define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
    #define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN

    //Don't disable this in order for the custom startup animation to work properly..
    #define RGB_MATRIX_KEYPRESSES
    #define RGB_MATRIX_KEYRELEASES

    #define RGB_MATRIX_LED_PROCESS_LIMIT 10
    #define RGB_MATRIX_LED_FLUSH_LIMIT 26

    //SK6812 Tweak
    #define WS2812_TRST_US 80

    // This allows VIA to control RGB Matrix settings in the 'Lighting' section.
    #define VIA_QMK_RGBLIGHT_ENABLE
#endif

// RGB light
#ifdef RGBLIGHT_ENABLE
    //RGB Matrix Config
    #define RGBW
    #define RGB_DI_PIN B2
    #define RGBLED_NUM 76
    #define RGBLIGHT_LIMIT_VAL 190
    #define RGB_MATRIX_STARTUP_SPD 64
    #define RGBLIGHT_SLEEP

    //#define RGBLIGHT_ANIMATIONS
    #define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CYCLE_LEFT_RIGHT
    #define RGBLIGHT_EFFECT_ALTERNATING
    #define RGBLIGHT_EFFECT_BREATHING
    #define RGBLIGHT_EFFECT_RAINBOW_MOOD
    #define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#endif
