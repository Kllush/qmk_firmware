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
_SYS	};		  // System control layer (reset, sleep, etc)

enum custom_keycodes {
  KC_MAKE = SAFE_RANGE,
  NEW_SAFE_RANGE  //use "NEW_SAFE_RANGE" for keymap specific codes
};

/* Keyboard & RGB matrix strip wiring / index
* ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┬───┐
* │0  │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 10│ 11│ 12│ 14    │15 │
* ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┼───┤
* │ 16  │18 │19 │20 │21 │22 │23 │24 │25 │26 │27 │28 │29 │30   │31 │
* ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┼───┤
* │ 32   │34 │35 │36 │37 │38 │39 │40 │41 │42 │43 │44 │  45    │47 │
* ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┼───┤
* │ 49     │50 │51 │52 │53 │54 │55 │56 │57 │58 │59 │61    │62 │63 │
* ├────┬───┴┬──┴─┬─┴───┴───┴┬──┴─┬─┴───┴──┬┴──┬┴──┬┴──┬───┼───┼───┤
* │ 64 │ 65 │ 67 │  69      │ 71 │  72    │74 │75 │   │77 │78 │79 │
* └────┴────┴────┴──────────┴────┴────────┴───┴───┘   └───┴───┴───┘
*/
/* Underglow - view from top down
* ┌───────────────────────────────────────────────────────────────┐
* │                                                               │
* │     80          81            82           83          84     │
* │                                                               │
* │                                                               │
* │                                                               │
* │                                                               │
* │                                                               │
* │     85          86            87           88          89     │
* │                                                               │
* └───────────────────────────────────────────────────────────────┘
*/


//LED TO MATRIX MAP
/* led_config_t g_led_config = {
    {
        // Key Matrix to LED Index
        {      0,      1,      2,      3,      4,      5,      6,      7,  8,      9, 10, 11,     12, NO_LED,     14,   15 },
        {     16, NO_LED,     18,     19,     20,     21,     22,     23, 24,     25, 26, 27,     28,     29,     30,   31 },
        {     32, NO_LED,     34,     35,     36,     37,     38,     39, 40,     41, 42, 43,     44,     45, NO_LED,   47 },
        { NO_LED,     49,     50,     51,     52,     53,     54,     55, 56,     57, 58, 59, NO_LED,     61,     62,   63 },
        {     64,     65, NO_LED,     67, NO_LED,     69, NO_LED,     71, 72, NO_LED, 74, 75, NO_LED,     77,     78,   79 }
    }, {
        // LED Index to Physical Position
         {0,0}, {15,0}, {30,0}, {45,0},  {60,0},  {75,0},  {90,0}, {105,0}, {120,0}, {135,0}, {150,0}, {165,0}, {180,0}, {203,0}, {224,0},
        {4,16},{23,16},{38,16},{53,16}, {68,16}, {83,16}, {98,16},{113,16},{128,16},{143,16},{158,16},{173,16},{188,16},{206,16},{224,16},
        {6,32},{26,32},{41,32},{56,32}, {71,32}, {86,32},{101,32},{116,32},{131,32},{146,32},{161,32},{176,32},{201,32},{224,32},
        {9,48},{34,48},{49,48},{64,48}, {79,48}, {94,48},{109,48},{124,48},{139,48},{154,48},{169,48},{189,48},{210,48},{224,48},
        {2,64},{21,64},{39,64},{96,64},{152,64},{171,64},{195,64},{210,64},{224,64}
        //underglow
        { 242,  64 }, { 216,  64 }, { 192,  64 }, { 176,  64 }, { 160,  64 }, { 144,  64 }, { 128,  64 }, { 112,  64 }, {  96,  64 }, {  80,  64 }, {  64,  64 }, {  48,  64 }, {  32,  64 }, {  16,  64 }, {   0,  64 },
        {   0,  16 }, {  16,  16 }, {  32,  16 },                                    {  88,   0 }, { 104,   0 }, { 120,   0 }, { 136,   0 },                                    { 176,  16 }, { 192,  16 }, { 216,  16 }
    }, {
        // LED Index to Flag
        1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
        1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
        1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
        1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
        1, 1, 1, 4, 1, 1, 1, 1, 1
        // Underglow LED's
        2,2,2,2,2,2,2,2,2,2
    } }; */


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_all( /* Base */
	KC_GESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,  KC_MUTE,
	KC_TAB,            KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,          KC_BSLS, LALT(KC_PSCR),
	MO(_SYS),             KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_INS,
	KC_LSFT,           KC_BSLS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_DEL,
	KC_LCTL,           KC_LGUI, KC_LALT,          KC_SPC,           MO(_FUNC),        KC_SPC,           LT(_RGB, KC_SLCK), MO(_FUNC),        KC_LEFT, KC_DOWN, KC_RGHT
  ),
  [_FUNC] = LAYOUT_all( /* FN */
	KC_TILD,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,_______, _______, _______,
	_______ , _______, _______, _______, _______, _______, _______, KC_P7, KC_P8, KC_P9, _______, _______, _______,          _______, LGUI(KC_PSCR),
	_______, _______, _______, _______, _______, _______, _______, KC_P4, KC_P5, KC_P6, _______, _______,          KC_PENT,          _______,
	_______, _______, _______, _______, _______, _______, _______, KC_P1, KC_P2, KC_P3, _______, _______, KC_NLCK,          KC_PGUP, _______,
	_______, _______, _______,          _______,          _______,          KC_P0,          KC_PDOT, _______,          KC_HOME, KC_PGDN, KC_END
  ),
  [_RGB] = LAYOUT_all(
    _______, _______, _______,   _______, _______, _______,   _______, _______, _______,   _______, _______, _______,  _______, _______, _______, RGB_TOG,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, RGB_MOD,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          RGB_RMOD,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RGB_VAI, RGB_M_SW,
    _______, _______, _______,          _______,          _______,          _______,          _______, _______,          RGB_SAD, RGB_VAD, RGB_SAI
  ),
  [_SYS] = LAYOUT_all(
    _______, _______, _______,   _______, _______, _______,   _______, _______, _______,   _______, _______, _______,  _______, _______, KC_SLEP, _______,
    KC_CAPS, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RESET, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_MAKE,          _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
    _______, _______, _______,          _______,          _______,          _______,          _______, _______,          _______, _______, _______
  )
};

//===================Encoder Functions===================//
void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            tap_code(KC_VOLD);
        } else {
            tap_code(KC_VOLU);
        }
    }
}
//===================Encoder Functions===================//

//===============USB Suspend Functions====================//
#ifdef RGB_MATRIX_ENABLE
void suspend_power_down_kb(void) {
  rgb_matrix_set_suspend_state(true);
  suspend_power_down_user(); }
void suspend_wakeup_init_kb(void) {
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
    case KC_MAKE:  // Compiles the firmware, and adds the flash command based on keyboard bootloader
            if (!record->event.pressed) {
            uint8_t temp_mod = get_mods();
            uint8_t temp_osm = get_oneshot_mods();
            clear_mods(); clear_oneshot_mods();
            SEND_STRING("make " QMK_KEYBOARD ":" QMK_KEYMAP);
    #ifndef FLASH_BOOTLOADER
            if ((temp_mod | temp_osm) & MOD_MASK_SHIFT)
    #endif
            {
                SEND_STRING(":flash");
            }
            if ((temp_mod | temp_osm) & MOD_MASK_CTRL) {
                SEND_STRING(" -j8 --output-sync");
            }
            tap_code(KC_ENT);
            set_mods(temp_mod);
        }
        break;

  }
  return process_record_keymap(keycode, record);
}
