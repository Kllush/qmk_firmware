#undef BACKLIGHT_PIN

#define DYNAMIC_MACRO_NO_NESTING
#define TAPPING_TERM_PER_KEY

#undef MATRIX_COLS
#define MATRIX_COLS 16

// Free up space
#define NO_ACTION_ONESHOT
#define NO_MUSIC_MODE
#define LAYER_STATE_8BIT
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE

#ifdef ENCODER_ENABLE
    #define ENCODERS_PAD_A { B3 } //MISO
    #define ENCODERS_PAD_B { B1 } //SCLK / SCK
#endif

#ifdef RGB_MATRIX_ENABLE
    #define RGBW
    //RGB Matrix Effects
    #define ENABLE_RGB_MATRIX_BREATHING

    //RGB Matrix Config
    #define DRIVER_LED_TOTAL 100                // The number of LEDs connected
    #define RGB_DI_PIN B2                       // Assign to pin B2 / MOSI on board
    #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 150
#endif

#ifdef RGBLIGHT_ENABLE
    #define RGBW
    #define RGB_DI_PIN B2
    #define RGBLED_NUM 85
    #define RGBLIGHT_LIMIT_VAL 200

    //#define RGBLIGHT_ANIMATIONS
    #define RGBLIGHT_EFFECT_ALTERNATING
    #define RGBLIGHT_EFFECT_BREATHING
    #define RGBLIGHT_EFFECT_RAINBOW_MOOD
    #define RGBLIGHT_EFFECT_RAINBOW_SWIRL

    //#define RGBLIGHT_DEFAULT_MODE RGBLIGHT_EFFECT_RAINBOW_SWIRL
#endif
