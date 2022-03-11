/* Copyright 2018 'mechmerlin'
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

//#include "quantum.h"
//#include "action.h"
#include "version.h"
//#include "eeprom.h"
//#include "eeconfig.h"
#include <lib/lib8tion/lib8tion.h>

//extern rgb_config_t rgb_matrix_config;

uint16_t     fade_timer;
uint16_t     boot_timer;
uint16_t     spam_timer;
uint16_t     spam_interval = 1000;
HSV          rgb_original_hsv;
int8_t       fade_status = 0;
int8_t       boot_status = 0;

static bool  RGB_MOD_FLAG;
static bool  LIGHTBAR_FLAG;

// Spam G and Q macros for lost ark
bool Q_SPAM;
bool G_SPAM;

// Custom RGB timeout timer
#if RGB_DISABLE_TIMEOUT == 0
    uint32_t     rgb_anykey_timeout;
    bool         timeout;
#endif

// Define custom values if not defined in config.h
#if STARTUP_ANIM_TIME < 0
#   error STARTUP_ANIM_TIME must be greater than 0
#elif !defined (STARTUP_ANIM_TIME)
#   define STARTUP_ANIM_TIME 2200
#endif

#if FADE_TIME < 0
#   error FADE_TIME must be greater than 0
#elif !defined (FADE_TIME)
#   define FADE_TIME 4
#endif

// End of custom values


enum keyboard_layers {
  _BASE = 0, 	// Base Layer
  _FUNC,     	// Function Layer
  _RGB,     	// RGB control Layer
  _SYS          // System control layer (reset, sleep, etc)
};

enum custom_keycodes {
  KC_MAKE = SAFE_RANGE, // Build QMK for keyboard
  VRSN,                 // Print QMK Version
  KC_TEST,              // Test purpose keycode
  KC_LOCK_W,            // Locking "W" key for autowalk, to be used with tap dance functions. Otherwise, you can just use KC_LOCK feature
  MON_OFF,              // Custom keycode to turn off monitors and timeout the backlight. To be used with nircmd on the host computer.
  EEP_RST_R,            // Wipe eeprom and reset keyboard
  KC_Q_SPAM,
  KC_G_SPAM,
  NEW_SAFE_RANGE,       // Use "NEW_SAFE_RANGE" for keymap specific codes
};

//============Tap Dance Functions===================//
#ifdef TAP_DANCE_ENABLE
    // Define a type for as many tap dance states as you need
    typedef enum {
        TD_NONE,
        TD_UNKNOWN,
        TD_SINGLE_TAP,
        TD_SINGLE_HOLD,
        TD_DOUBLE_TAP,
        TD_DOUBLE_HOLD
    } td_state_t;

    typedef struct {
        bool is_press_action;
        uint8_t state;
    } td_tap_t;

    // Tap dance keycodes
    enum {
        TD_MUTE,
        TD_SYS_QL
    };

    // Function associated with all tap dances
   td_state_t cur_dance(qk_tap_dance_state_t *state);

    // Functions associated with individual tap dances
    void ql_finished(qk_tap_dance_state_t *state, void *user_data);
    void ql_reset(qk_tap_dance_state_t *state, void *user_data);

    // Determine the current tap dance state
    td_state_t cur_dance(qk_tap_dance_state_t *state) {
        if (state->count == 1) {
            if (!state->pressed) return TD_SINGLE_TAP;
            else return TD_SINGLE_HOLD;
        } else if (state->count == 2) {
            if (!state->pressed) return TD_DOUBLE_TAP;
            else return TD_DOUBLE_HOLD;
        }
        else return TD_UNKNOWN;
    }

    // Initialize tap structure associated with example tap dance key
    static td_tap_t ql_tap_state = {
        .is_press_action = true,
        .state = TD_NONE
    };

    // Functions that control what our tap dance key does
    void ql_finished(qk_tap_dance_state_t *state, void *user_data) {
        ql_tap_state.state = cur_dance(state);
        switch (ql_tap_state.state) {
            case TD_SINGLE_TAP:
                if (host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK)) {
                    tap_code(KC_CAPS); // Disable caps if it's enabled
                }
                break;
            case TD_SINGLE_HOLD:
                break;
            case TD_DOUBLE_TAP:
                //if (host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK)) {}       // Enable caps if it's disabled
                //else {
                    tap_code(KC_CAPS);
                //}
                break;
            case TD_DOUBLE_HOLD:
                layer_on(_SYS);
                break;
        }
    }

    // Function that controls the reset tap dance
    void ql_reset(qk_tap_dance_state_t *state, void *user_data) {
        // If the key was held down and now is released then switch off the layer
        if (ql_tap_state.state >= TD_DOUBLE_HOLD) {
            layer_off(_SYS);
        }
        ql_tap_state.state = TD_NONE;
    }

    // Associate our tap dance key with its functionality
    qk_tap_dance_action_t tap_dance_actions[] = {
        [TD_MUTE] = ACTION_TAP_DANCE_DOUBLE(KC_MUTE, KC_SLCK),                          // Tap once for mute, twice for mute discord (scroll lock)
        [TD_SYS_QL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished, ql_reset),        // Caps lock/System layer dance
    };
#endif
//============Tap Dance Functions End================//


//========Keyboard & RGB matrix strip wiring / index====//
/*
*   ___________________________________
*  /   0          1  2  3  4  5  6     \_____________          <-- Light bar
* ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┬───┐
* │ 7 │ 8 │ 9 │ 10│ 11│ 12│ 13│ 14│ 15│ 16│ 17│ 18│ 19│ 20    │21 │
* ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┼───┤
* │ 36  │35 │34 │33 │32 │31 │30 │29 │28 │27 │26 │25 │24 │23   │22 │
* ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┼───┤
* │ 37   │38 │39 │40 │41 │42 │43 │44 │45 │46 │47 │48 │  49    │50 │
* ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┼───┤
* │ 64     │63 │62 │61 │60 │59 │58 │57 │56 │55 │54 │53    │51 │51 │
* ├────┬───┴┬──┴─┬─┴───┴───┴┬──┴─┬─┴───┴──┬┴──┬┴──┬┴──┬───┼───┼───┤
* │ 65 │ 66 │ 67 │  68      │ 69 │  70    │71 │72 │   │73 │74 │75 │
* └────┴────┴────┴──────────┴────┴────────┴───┴───┘   └───┴───┴───┘
*/

#ifdef RGB_MATRIX_ENABLE
    //LED TO MATRIX MAP
    led_config_t g_led_config = { {
            // Key Matrix to LED Index
            {      7,      8,      9,     10,     11,     12,     13,     14,     15, 16, 17, 18,     19,     20,     21 },
            {     36,     35,     34,     33,     32,     31,     30,     29,     28, 27, 26, 25,     24,     23,     22 },
            {     37,     38,     39,     40,     41,     42,     43,     44,     45, 46, 47, 48, NO_LED,     49,     50 },
            {     64,     63,     62,     61,     60,     59,     58,     57,     56, 55, 54, 53, NO_LED,     52,     51 },
            {     65,     66,     67,     68, NO_LED,     69, NO_LED,     70, NO_LED, 71, 72, 73, NO_LED,     74,     75 }
        }, {
            // LED Index to Physical Position **BASED ON KEY MATRIX TO LED INDEX (back and fourth snake in this case)
            {10,0},{50,0},{62,0},{74,0},{86,0},{98,0},{110,0},
            {0,12},{15,12},{30,12},{45,12},{60,12},{75,12},{90,12},{105,12},{120,12},{135,12},{150,12},{165,12},{180,12},{203,12},{224,12},
            {224,25},{203,25},{180,25},{165,25},{150,25},{135,25},{120,25},{105,25},{90,25},{75,25},{60,25},{45,25},{30,25},{15,25},{0,25},
            {0,38},{25,38},{40,38},{55,38},{70,38},{85,38},{100,38},{115,38},{130,38},{145,38},{160,38},{175,38},{201,38},{224,38},
            {224,51},{210,51},{189,51},{169,51},{154,51},{139,51},{120,51},{105,51},{90,51},{75,51},{60,51},{45,51},{25,51},{5,51},
            {2,64},{21,64},{39,64},{70,64},{90,64},{120,64},{150,64},{165,64},{190,64},{205,64},{224,64}
        }, {
            // LED Index to Flag
            2,2,2,2,2,2,2, // Lightbar
            1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1,
            1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
            1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    1, 1,
            1,    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 4, 1,
            1, 1, 1,    1,    1,    1,    1, 1, 4, 4, 4,
        }
    };
#endif
//====Keyboard & RGB matrix strip wiring / index end====//


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_all( /* Base */
	KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,  TD(TD_MUTE),
	KC_TAB,            KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,          KC_BSLS, KC_DEL,
	TD(TD_SYS_QL),             KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_INS,
	KC_LSFT,           KC_BSLS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   LALT(KC_PSCR),
	KC_LCTL,           KC_LGUI, KC_LALT,          KC_SPC,           MO(_FUNC),        KC_SPC,           MO(_RGB), MO(_FUNC),        KC_LEFT, KC_DOWN, KC_RIGHT
  ),
  [_FUNC] = LAYOUT_all( /* FN */
	KC_TILD,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, LCTL(KC_BSPC), _______, _______,
	KC_GRV , _______, KC_UP, _______, _______, _______, _______, KC_P7, KC_P8, KC_P9, _______, _______, _______,          LGUI(KC_PAUS), DM_PLY1,
	_______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, KC_P4, KC_P5, KC_P6, _______, _______,          KC_PENT,          DM_PLY2,
	_______, _______, _______, _______, _______, _______, _______, KC_P1, KC_P2, KC_P3, _______, _______, KC_NLCK,          KC_PGUP, KC_PSCR,
	_______, GUI_TOG, _______,          _______,          _______,          KC_P0,          KC_PDOT, _______,          KC_HOME, KC_PGDN, KC_END
  ),
  [_RGB] = LAYOUT_all(
    _______, _______, _______,   _______, _______, _______,   _______, _______, _______,   _______, _______, _______,  _______, KC_DEL, _______, RGB_TOG,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, RGB_MOD,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          RGB_SPI,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RGB_VAI, RGB_HUI,
    _______, _______, _______,          _______,          _______,          _______,          _______, _______,          RGB_SAD, RGB_VAD, RGB_SAI
  ),
  [_SYS] = LAYOUT_all(
    _______, _______, _______, _______, _______, _______,   _______, _______, _______,   _______, _______, _______,  _______, _______, _______, KC_SLEP,
    _______, KC_Q_SPAM, KC_LOCK_W, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          VRSN, DM_REC1,
    _______, _______, _______, _______, _______, KC_G_SPAM, _______, _______, _______, _______, _______, _______,          KC_MAKE,          DM_REC2,
    _______, _______, _______, _______, _______, EEP_RST_R, RESET, NK_TOGG, _______, _______, _______, _______, _______,          _______, DM_RSTP,
    _______, GUI_TOG, _______,          _______,          MON_OFF,          _______,          _______, _______,          _______, _______, _______
  )
};


//===============Custom Functions=========================//
void rgb_matrix_fade_in(void) {
    fade_timer = timer_read();
    rgb_original_hsv = rgb_matrix_config.hsv;  //grab current V value to restore
    rgb_matrix_sethsv_noeeprom( rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, 0);
    fade_status = 1;
}

void rgb_matrix_fade_out(void) {
    fade_timer = timer_read();
    rgb_original_hsv = rgb_matrix_config.hsv;  //grab current V value to restore
    fade_status = -1;
}

void rgb_matrix_fade_out_noeeprom(void) {
    fade_timer = timer_read();
    rgb_original_hsv = rgb_matrix_config.hsv;  //grab current V value to restore
    fade_status = -2;
}

// Enable splash math function if ENABLE_RGB_MATRIX_SPLASH was not defined
#if !defined (ENABLE_RGB_MATRIX_SPLASH)
    // Splash animation math
    HSV SPLASH_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
        uint16_t effect = tick - dist;
        if (effect > 255) effect = 255;
        hsv.h += effect;
        hsv.v = qadd8(hsv.v, 255 - effect);
        return hsv;
    }
#endif

#if defined (ENABLE_RGB_MATRIX_SPLASH)
    HSV SPLASH_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick);
#endif
// End of splash math functions

// Boot animation, run only if RGB_MATRIX_ENABLED is defined and if the matrix is enabled.
void rgb_matrix_boot_anim(uint8_t boot_anim) {
    #if defined (RGB_MATRIX_ENABLE)
        if (rgb_matrix_config.enable) {
            rgb_matrix_sethsv_noeeprom( rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, RGB_MATRIX_MAXIMUM_BRIGHTNESS);   // Set brightness to maximum allowed before playing animation
            rgb_matrix_set_speed_noeeprom(64);                                                                              // Set animation speed to default before playing animation
            boot_timer = timer_read();
            boot_status = boot_anim;
        }
    #endif
} // End of boot animation

// Infinite repeating key macro
//void

// End of infinite repeating key

//===============Custom Functions End=====================//


//===================Encoder Functions===================//
#ifdef ENCODER_ENABLE
    bool encoder_update_user(uint8_t index, bool clockwise) {
        if (index == 0) {
            // Per layer encoder
            switch (biton32(layer_state)) {
                case _FUNC:                     // Control brightness of monitors if on _FUNC layer with encoder
                    if (clockwise) {            //      Requires monitor compatible with DDC/CI
                        tap_code(KC_BRID);
                    } else {
                        tap_code(KC_BRIU);
                    }
                    break;
                case _RGB:
                    break;
                case _SYS:
                    break;
                default:                        // Control volume with encoder on default layer
                    if (clockwise) {
                        tap_code(KC_VOLD);
                    } else {
                        tap_code(KC_VOLU);
                    }
                    break;
            }
        }
        return false;
    }
#endif
//===================Encoder Functions End=============//


//=================Keycode Functions ================//
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    // Custom timeout code to add fades - FADE IN
    #if RGB_DISABLE_TIMEOUT == 0
        #if (defined (RGB_MATRIX_ENABLE)) || (defined (RGBLIGHT_ENABLE))
            if (record->event.pressed) {
                rgb_anykey_timeout = timer_read32();    // Reset timeout timer on any key press

                if (timeout == true) {                  // Reset timer and turn on backlight to last mode if timed-out
                    timeout = false;
                    rgb_matrix_enable_noeeprom();
                    rgb_matrix_fade_in();
                }
            }
        #endif
    #endif // End of custom timeout code - FADE IN

    // Custom keycodes - On key release
    if (!record->event.pressed) {
        switch (keycode) {
        case KC_MAKE:   // Compiles the firmware, and adds the flash command based on keyboard bootloader
            clear_mods();
            if (host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK)) tap_code(KC_CAPS); // Disable caps if it's enabled
            SEND_STRING("make " QMK_KEYBOARD ":" QMK_KEYMAP);
            tap_code(KC_ENT);
            return false;
        case VRSN:      // Prints QMK firmware version information
            send_string_with_delay_P(PSTR(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION ", Built on: " QMK_BUILDDATE), TAP_CODE_DELAY);
            return false;
        case MON_OFF:
            SEND_STRING(SS_TAP(X_LGUI) SS_DELAY(500) "mon" SS_DELAY(500) SS_TAP(X_ENT));

            // Run a shutdown animation before kb goes to sleep
            #if (defined (RGB_MATRIX_ENABLE)) || (defined (RGBLIGHT_ENABLE))
                rgb_matrix_boot_anim(2);
            #endif
            return false;
        case EEP_RST_R:
            eeconfig_init();
            wait_ms(250);
            reset_keyboard();
            return false;
        case RGB_TOG:   // Override original RGB_TOG function and add fades
            #if (defined (RGB_MATRIX_ENABLE)) || (defined (RGBLIGHT_ENABLE))
                if (rgb_matrix_is_enabled()) {
                    rgb_matrix_fade_out();
                } else if (!rgb_matrix_is_enabled()) {
                    rgb_matrix_enable();
                    rgb_matrix_fade_in();
                }
            #endif
            return false;
        case KC_G_SPAM: // Spam keys for idle farming in lost ark (Skill "Q" and "G" farm)
            G_SPAM ^= 1;
            spam_timer = timer_read();
            return false;
        case KC_Q_SPAM:
            Q_SPAM ^= 1;
            spam_timer = timer_read();
            return false;
        case KC_G:      // Easily cancel key spam by pressing the normal key again
            G_SPAM = 0;
            return true;
        case KC_Q:
            Q_SPAM = 0;
            return true;
        default:
            return true;
        }
    } else {    // On key press
        switch (keycode) {
        case KC_LOCK_W:   // Locking "W" key
            register_code(KC_W);
            return false;
        case RGB_MODE_FORWARD ... RGB_SPD:  // Add RGB_MOD_FLAG = true to all RGB modification keys.
            RGB_MOD_FLAG = true;            //   This is to let the per key indicator know to stop if the RGB settings are modified so
            return true;                    //   the user can see the changes again without the layer indicator in the way
        default:
            return true;
        }
    } // End of custom keycodes
}
//==============Keycode Functions End=============//
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _BASE:
            if (RGB_MOD_FLAG != false) { RGB_MOD_FLAG = false; }    // Set RGB_MOD_FLAG to false on the default layer
            break;
    }
    return state;
}
//==============Layer indicator code==============//



//==============Layer indicator code end==========//


//============Tapping Term per Key================//
#ifdef TAPPING_TERM_PER_KEY
    uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
        switch (keycode) {
            case TD(TD_MUTE):   // Tapping term for encoder mute. Since encoder is more difficult to double tap, increase the tapping term
                return 300;
            case TD(TD_SYS_QL): // Tapping term for _SYS layer tap dance
                return 175;
            default:            // Default tapping term
                return 200;
        }
    }
#endif
//===========Tapping Term per Key End==============//


/* Boot_status
*   1 = Run splash boot animation
*   0 = Don't run anim / anim complete
*/
//==========Per Layer RGB Matrix indicators========//
void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    void rgb_matrix_boot_anim_runner(uint8_t originx, uint8_t originy) {
        for (uint8_t i = led_min; i < led_max; i++) {
                    HSV hsv = rgb_matrix_config.hsv;
                    hsv.v   = 0;
                    for (uint8_t j = 0; j < 1; j++) {
                        int16_t  dx   = g_led_config.point[i].x - originx;     // X origin of splash animation
                        int16_t  dy   = g_led_config.point[i].y - originy;     // y origin of splash animation
                        uint8_t  dist = sqrt16(dx * dx + dy * dy);
                        uint16_t tick = scale16by8(timer_elapsed(boot_timer), qadd8(rgb_matrix_config.speed, 1));
                        hsv           = SPLASH_math(hsv, dx, dy, dist, tick);
                    }
                    hsv.v   = scale8(hsv.v, rgb_matrix_config.hsv.v);
                    RGB rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
                }
    }

    // Boot animation Code
    switch(boot_status) {
    case 1:
        if (timer_elapsed(boot_timer) >= STARTUP_ANIM_TIME) {                                                   // If timer is > boot animation time, load the saved RGB mode and fade in
            rgb_matrix_sethsv_noeeprom( rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_original_hsv.v);  // Reset HSV.v to original value
            rgb_matrix_set_speed_noeeprom(rgb_matrix_config.speed);                                             // Reset speed to original value
            eeprom_read_block(&rgb_matrix_config, EECONFIG_RGB_MATRIX, sizeof(rgb_matrix_config));
            rgb_matrix_mode_noeeprom(rgb_matrix_config.mode);                                                   // Load original mode
            rgb_matrix_fade_in();                                                                               // Fade in after boot animation is complete
            boot_status = 0;
        } else {                                                                                                // Otherwise, run boot animation
            rgb_matrix_boot_anim_runner(30,0);
        }
        return;
    case 2:
        if (timer_elapsed(boot_timer) >= STARTUP_ANIM_TIME) {                                                   // If timer is > boot animation time, load the saved RGB mode and fade in
            rgb_matrix_sethsv_noeeprom( rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_original_hsv.v);  // Reset HSV.v to original value
            rgb_matrix_set_speed_noeeprom(rgb_matrix_config.speed);                                             // Reset speed to original value

            timeout = true;
            rgb_matrix_disable_noeeprom();

            boot_status = 0;
        } else {                                                                                                // Otherwise, run boot animation
            rgb_matrix_boot_anim_runner(112,64);
        }
        return;
    }   // End boot animation code

    // Layer indicator code
    if (get_highest_layer(layer_state) > 0 && !RGB_MOD_FLAG) {
        uint8_t layer = get_highest_layer(layer_state);

        // Lightbar layer indicator
        HSV hsv = rgb_matrix_config.hsv;
        hsv.s   = 255;  // Ensure RGB colors are full saturation regardless of user's setting

        // Layer color settings
        //  LIGHTBAR_FLAG
        //
        //  TRUE    = custom indicator color
        //  FALSE   = no custom indicator color
        switch (layer) {
            case _FUNC:
                LIGHTBAR_FLAG = false;
                if (LIGHTBAR_FLAG) { hsv.h = 0;} // Red but disabled so animation shows through
                break;
            case _RGB:
                LIGHTBAR_FLAG = true;
                if (LIGHTBAR_FLAG) { hsv.h = 50;} // Yellow
                break;
            case _SYS:
                LIGHTBAR_FLAG = true;
                if (LIGHTBAR_FLAG) { hsv.h = 0;} // Red
                break;
            default:
                LIGHTBAR_FLAG = false;
                break;
        }

        //apply the colors to the layers, if configured. Otherwise, the user's RGB mode will show through
        RGB rgb = hsv_to_rgb(hsv);

        if (LIGHTBAR_FLAG) {
            for (uint8_t i = led_min; i <= led_max; i++) {
                if (HAS_FLAGS(g_led_config.flags[i], 0x02)) { // 0x02 == LED_FLAG_UNDERGLOW
                    RGB_MATRIX_INDICATOR_SET_COLOR(i, rgb.r, rgb.g, rgb.b);
                }
            }
        } // End of lightbar layer indicator

        // Per configured key indicator
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                // Comparing LED index to matrix for configured keys or override keys
                if (index >= led_min && index <= led_max && index != NO_LED) {

                    uint16_t keycheck = keymap_key_to_keycode(layer, (keypos_t){col,row});
                    HSV hsv = rgb_matrix_config.hsv;
                    hsv.s   = 255;  // Ensure RGB colors are full saturation regardless of user's setting

                    // If key is configured, light it up
                    if (keycheck > KC_TRNS) {

                        // Per key overrides
                        switch (keycheck) {
                            case RESET:
                                hsv.h = 0;  // RED
                                break;
                            case LCTL(KC_BSPC):
                                hsv.h = 0;  // RED
                                break;
                            case EEP_RST_R:
                                hsv.h = 50; // YELLOW
                                break;
                            // Keep at bottom
                            case NK_TOGG:
                                if (keymap_config.nkro == 1) { hsv.h = 85; } // GREEN if nkro is enabled
                                else
                            case GUI_TOG:
                                if (keymap_config.no_gui == 1) { hsv.h = 0; } // RED if GUI is disabled
                                else
                            default:        // Default per key color
                                hsv.s = 0;  // Set per key lights to white and respect the user's hsv.v value
                        }

                        // Make the key lights a bit brighter
                        if (hsv.v < (RGB_MATRIX_MAXIMUM_BRIGHTNESS - 30)) { hsv.v += 30; }
                        else { hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS; }

                        rgb = hsv_to_rgb(hsv);
                        RGB_MATRIX_INDICATOR_SET_COLOR(index, rgb.r, rgb.g, rgb.b);
                    }
                    else {  // If key is not configured, light it up differently or not at all.
                        RGB_MATRIX_INDICATOR_SET_COLOR(index, 0, 0, 0); //If this line is commented out, the original animation will show through. However, freezes my keyboard after 2s.
                    }
                } // End of comparison code
            }
        } // End of per configured key indicator

    } // End of layer indicator code

    // Caps lock / num lock code
    if (host_keyboard_led_state().caps_lock) {
        HSV hsv = rgb_matrix_config.hsv;
        hsv.s = 0;                                                                  // Use white LED for indicator

        if (hsv.v < 100) { hsv.v = 100; }                                           // Ensure the caps/num lock has a minimum brightness and doesn't exceed the defined max brightness
        else if (hsv.v < (RGB_MATRIX_MAXIMUM_BRIGHTNESS - 30)) { hsv.v += 30; }     //   Also makes the caps/num lock a bit brighter than the rest of the LED's
        else { hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS; }


        RGB rgb = hsv_to_rgb(hsv);
        RGB_MATRIX_INDICATOR_SET_COLOR(3, rgb.r, rgb.g, rgb.b);                     // Assuming caps lock is at led #3
    }
    if (host_keyboard_led_state().num_lock) {
        HSV hsv = rgb_matrix_config.hsv;
        hsv.s = 0;

        if (hsv.v < 100) { hsv.v = 100; }
        else if (hsv.v < (RGB_MATRIX_MAXIMUM_BRIGHTNESS - 30)) { hsv.v += 30; }
        else { hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS; }

        RGB rgb = hsv_to_rgb(hsv);
        RGB_MATRIX_INDICATOR_SET_COLOR(5, rgb.r, rgb.g, rgb.b);                     // Assuming num lock is at led #5
    } // End of caps/num lock code
}


/* fade_status
*    1 = fade in
*   -1 = fade out
*   -2 = fade out, no eeprom
*/
//=============================================//
void rgb_matrix_indicators_user (void) {

    // Fade animation code
    HSV hsv = rgb_matrix_get_hsv();
    switch(fade_status) {
        case 1:     // Fade in code, slows down fade in to every FADE_TIME ms
            if ((timer_elapsed(fade_timer) > FADE_TIME) && (hsv.v <= rgb_original_hsv.v)) {
                rgb_matrix_increase_val_noeeprom();
                fade_timer = timer_read();
            } else if (hsv.v >= rgb_original_hsv.v) {   // End code, when brightness has reached max configured
                fade_status = 0;
                rgb_matrix_sethsv( rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_original_hsv.v);
            }
            return;
        case -1:    // Fade out code, slows down fade in to every FADE_TIME ms
            if ((timer_elapsed(fade_timer) > FADE_TIME) && (hsv.v > 0)) {
                rgb_matrix_decrease_val_noeeprom();
                fade_timer = timer_read();
            } else if (hsv.v == 0) {    // End code, when brightness has reached 0
                fade_status = 0;
                rgb_matrix_sethsv_noeeprom( rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_original_hsv.v);
                rgb_matrix_disable();
            }
            return;
        case -2:    // Fade out code, no EEPROM. Slows down fade in to every FADE_TIME ms
            if ((timer_elapsed(fade_timer) > FADE_TIME) && (hsv.v > 0)) {
                rgb_matrix_decrease_val_noeeprom();
                fade_timer = timer_read();
            } else if (hsv.v == 0) {    // End code, when brightness has reached max configured
                fade_status = 0;
                rgb_matrix_sethsv_noeeprom( rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_original_hsv.v);
                rgb_matrix_disable_noeeprom();
            }
            return;
    } // End of fade animation code

    // Custom RGB timeout code that includes fades - FADE OUT
    #if RGB_DISABLE_TIMEOUT == 0
        if (rgb_matrix_config.enable) {
            #if (defined (RGB_MATRIX_ENABLE)) || (defined (RGBLIGHT_ENABLE))
                if ((timer_elapsed32(rgb_anykey_timeout) > UINT32_MAX)) {
                    rgb_anykey_timeout = UINT32_MAX;
                } else if ((timer_elapsed32(rgb_anykey_timeout) > (uint32_t)RGB_CUSTOM_TIMEOUT_DELAY) && (rgb_matrix_config.enable)) {
                    rgb_matrix_fade_out_noeeprom();
                    timeout = true;
                }
            #endif
        }
    #endif // End of custom RGB timeout code - FADE OUT
}

//==========Per Layer RGB Matrix indicators========//


//==========Keyboard init/suspend functions========//
void keyboard_post_init_user(void) {
    // Start timer for custom rgb timeout
    #if (RGB_DISABLE_TIMEOUT == 0)
        #if (defined (RGB_MATRIX_ENABLE)) || (defined (RGBLIGHT_ENABLE))
            rgb_anykey_timeout = timer_read32();
        #endif
    #endif
}

// code will run on keyboard wakeup
void suspend_wakeup_init_user(void) {
    // Fade in RGB when first plugging in kb or on resume from sleep
    #if (defined (RGB_MATRIX_ENABLE)) || (defined (RGBLIGHT_ENABLE))
        rgb_matrix_boot_anim(1);
    #endif
}
//=======Keyboard init/suspend functions End=======//

//=======Matrix Scan User==========================//
void matrix_scan_user(void) {
    if (timer_elapsed(spam_timer) >= spam_interval) {
        if (G_SPAM) {
            spam_timer = timer_read();
            tap_code(KC_G);
        }
        if (Q_SPAM) {
            spam_timer = timer_read();
            tap_code(KC_Q);
        }
    }
}
//=======Matrix Scan User==========================//
