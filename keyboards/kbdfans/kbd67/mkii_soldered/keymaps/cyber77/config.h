#undef BACKLIGHT_PIN
#undef RGB_DI_PIN

#define DYNAMIC_MACRO_NO_NESTING

#define ENCODERS_PAD_A { B2 }
#define ENCODERS_PAD_B { B1 }

#define TAPPING_TERM 240

#ifdef RGB_MATRIX_ENABLE
    //RGB Matrix Effects
    #define RGB_MATRIX_KEYPRESSES                                // Keypress effects
    #define RGB_MATRIX_FRAMEBUFFER_EFFECTS                       // Frame buffer effects
    #define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CYCLE_LEFT_RIGHT  //default mode
    //disable standard effects
    #define DISABLE_RGB_MATRIX_ALPHAS_MODS
    #define DISABLE_RGB_MATRIX_BAND_SAT
    #define DISABLE_RGB_MATRIX_BAND_VAL
    #define DISABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
    #define DISABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
    #define DISABLE_RGB_MATRIX_BAND_SPIRAL_SAT
    #define DISABLE_RGB_MATRIX_BAND_SPIRAL_VAL
    #define DISABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
    #define DISABLE_RGB_MATRIX_RAINDROPS
    //disable keypress effects
    #define DISABLE_RGB_MATRIX_SOLID_REACTIVE
    #define DISABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
    #define DISABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
    #define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
    #define DISABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
    #define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
    #define DISABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
    #define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXU
    #define DISABLE_RGB_MATRIX_MULTISPLASH

    //RGB Matrix Config
    #define RGB_DISABLE_WHEN_USB_SUSPENDED true // turn off effects when suspended
    #define DRIVER_LED_TOTAL 90                 // The number of LEDs connected
    #define RGB_DI_PIN E2                       // Assign to pin E2 on board
    #define RGBLIGHT_SLEEP
    #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200
    #define RGB_MATRIX_STARTUP_VAL 200
    #define RGB_MATRIX_STARTUP_SPD 64
#endif
