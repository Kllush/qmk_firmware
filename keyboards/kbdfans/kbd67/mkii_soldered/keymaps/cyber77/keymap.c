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

#include "quantum.h"
#include "action.h"
#include "version.h"

enum keyboard_layers {
  _BASE = 0, 	// Base Layer
  _FUNC,     	// Function Layer
  _RGB,     	// RGB control Layer
  _SYS          // System control layer (reset, sleep, etc)
};

enum custom_keycodes {
  KC_MAKE = SAFE_RANGE,
  KC_WINLOCK,
  NEW_SAFE_RANGE,  //use "NEW_SAFE_RANGE" for keymap specific codes
};


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
                if (host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK)) {}
                else {
                    tap_code(KC_CAPS); // Enable caps if it's enabled
                }
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


/* Keyboard & RGB matrix strip wiring / index
*
*
*   ___________________________________
*  /           0  1  2  3  4  5  6     \_____________          <-- Light bar
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
            {      1,      2,      3,     4,       5,     12,     13,     14, 15,     16, 17, 18,     19,     20, NO_LED,   22 },
            {     37, NO_LED,     36,     35,     34,     33,     32,     31, 30,     29, 28, 27,     26,     25,     24,   23 },
            {     38, NO_LED,     39,     40,     41,     42,     43,     44, 45,     46, 47, 48,     49,     50, NO_LED,   52 },
            {     68, NO_LED,     66,     65,     64,     63,     62,     61, 60,     59, 58, 57, NO_LED,     55,     54,   53 },
            {     69,     70, NO_LED,     72,     73, NO_LED,     75, NO_LED, 77, NO_LED, 79, 80, NO_LED,     82,     83,   84 }
        }, {
            // LED Index to Physical Position
            {0,0} , {15,0}, {30,0}, {45,0},  {60,0},  {75,0},  {90,0}, {105,0}, {120,0}, {135,0}, {150,0}, {165,0}, {180,0}, {203,0}, {224,0},
            {4,16},{23,16},{38,16},{53,16}, {68,16}, {83,16}, {98,16},{113,16},{128,16},{143,16},{158,16},{173,16},{188,16},{206,16},{224,16},
            {6,32},{26,32},{41,32},{56,32}, {71,32}, {86,32},{101,32},{116,32},{131,32},{146,32},{161,32},{176,32},{201,32},{224,32},
            {9,48},{34,48},{49,48},{64,48}, {79,48}, {94,48},{109,48},{124,48},{139,48},{154,48},{169,48},{189,48},{210,48},{224,48},
            {2,64},{21,64},{39,64},{96,64},{152,64},{171,64},{195,64},{210,64},{215,64},{220,64},{224,64}
        }, {
            // LED Index to Flag
            2,2,2,2,2,2, // Lightbar
            1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
            1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
            1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
            1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
            1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1
        }
    };
#endif


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_all( /* Base */
	KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,  TD(TD_MUTE),
	KC_TAB,            KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,          KC_BSLS, KC_DEL,
	TD(TD_SYS_QL),             KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_INS,
	KC_LSFT,           KC_BSLS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   LALT(KC_PSCR),
	KC_LCTL,           KC_LGUI, KC_LALT,          KC_SPC,           MO(_FUNC),        KC_SPC,           MO(_RGB), MO(_FUNC),        KC_LEFT, KC_DOWN, KC_RGHT
  ),
  [_FUNC] = LAYOUT_all( /* FN */
	KC_TILD,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,_______, _______, _______,
	KC_GRV , _______, KC_UP, _______, _______, _______, _______, KC_P7, KC_P8, KC_P9, _______, _______, _______,          LGUI(KC_PAUS), DM_PLY1,
	_______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, KC_P4, KC_P5, KC_P6, _______, _______,          KC_PENT,          DM_PLY2,
	_______, _______, _______, _______, _______, _______, _______, KC_P1, KC_P2, KC_P3, _______, _______, KC_NLCK,          KC_PGUP, KC_PSCR,
	_______, GUI_TOG, _______,          _______,          _______,          KC_P0,          KC_PDOT, _______,          KC_HOME, KC_PGDN, KC_END
  ),
  [_RGB] = LAYOUT_all(
    _______, _______, _______,   _______, _______, _______,   _______, _______, _______,   _______, _______, _______,  _______, _______, _______, RGB_TOG,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, RGB_MOD,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          RGB_M_SW,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RGB_VAI, RGB_HUI,
    _______, _______, _______,          _______,          _______,          _______,          _______, _______,          RGB_SAD, RGB_VAD, RGB_SAI
  ),
  [_SYS] = LAYOUT_all(
    _______, _______, _______, _______, _______, _______,   _______, _______, _______,   _______, _______, _______,  _______, EEP_RST, _______, KC_SLEP,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, DM_REC1,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_MAKE,          DM_REC2,
    _______, _______, _______, _______, _______, _______, RESET, NK_TOGG, _______, _______, _______, _______, _______,          _______, DM_RSTP,
    _______, GUI_TOG, _______,          _______,          _______,          _______,          _______, _______,          _______, _______, _______
  )
};

//===================Encoder Functions===================//
#ifdef ENCODER_ENABLE
    bool encoder_update_user(uint8_t index, bool clockwise) {
        if (index == 0) {
            // Per layer encoder
            switch (biton32(layer_state)) {
                case _FUNC:
                    if (clockwise) {
                        tap_code(KC_BRID);
                    } else {
                        tap_code(KC_BRIU);
                    }
                    break;
                case _RGB:
                    break;
                case _SYS:
                    break;
                default:
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
//===================Encoder Functions===================//

//===============USB Suspend Functions====================//

#ifdef RGB_MATRIX_ENABLE
  void suspend_power_down_user(void) {
    rgb_matrix_set_suspend_state(true);
    suspend_power_down_user(); }
  void suspend_wakeup_init_user(void) {
    rgb_matrix_set_suspend_state(false);
    suspend_wakeup_init_user(); }
#endif
//===============USB Suspend Functions====================//

__attribute__ ((weak))
bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
  return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // Keep at bottom
    case KC_MAKE:  // Compiles the firmware, and adds the flash command based on keyboard bootloader
        if (!record->event.pressed) {
            clear_mods();
            if (host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK)) tap_code(KC_CAPS); // Disable caps if it's enabled
            SEND_STRING("make " QMK_KEYBOARD ":" QMK_KEYMAP);
            tap_code(KC_ENT);
        }
        break;
  }
  return process_record_keymap(keycode, record);
}

// Tapping term per key
#ifdef TAPPING_TERM_PER_KEY
    uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
        switch (keycode) {
            case TD(TD_MUTE):
                return 250;
            case TD(TD_SYS_QL):
                return 200;
            default:
                return 200;
        }
    }
#endif

// Layer state handling
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _RGB:
        break;
    case _SYS:
        break;
    default: //  for any other layers, or the default layer
        break;
    }
  return state;
}
