/* Copyright 2022 bdtc123
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

enum my_keycodes {
    RMT = SAFE_RANGE,
    RMS,
    RMIH,
    RMDH,
    RMIS,
    RMDS,
    RMIV,
    RMDV
};
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case RMT:
      if (record->event.pressed)
      {rgb_matrix_toggle();
      }
      return false;
    case RMS:
      if (record->event.pressed)
      {rgb_matrix_step();
      }
      return false;
      case RMIH:
      if (record->event.pressed)
      {rgb_matrix_increase_hue();
      }
      return false;
      case RMDH:
      if (record->event.pressed)
      {rgb_matrix_decrease_hue();
      }
      return false;
      case RMIS:
      if (record->event.pressed)
      {rgb_matrix_increase_sat();
      }
      return false;
      case RMDS:
      if (record->event.pressed)
      {rgb_matrix_decrease_sat();
      }
      return false;
      case RMIV:
      if (record->event.pressed)
      {rgb_matrix_increase_val();
      }
      return false;
      case RMDV:
      if (record->event.pressed)
      {rgb_matrix_decrease_val();
      }
      return true;
      default:
      return true;
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        QK_GESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,  KC_MPLY,  KC_VOLD, KC_VOLU,
        KC_TAB,      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_CAPS,       KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT,          KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_RSFT,        KC_UP,   MO(1),
        KC_LCTL,   KC_LGUI,   KC_LALT,                       KC_SPC,                              KC_RALT, KC_RCTL,   KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [1] = LAYOUT(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  RESET,  _______, KC_MPRV, KC_MNXT,
        _______,     RMT,  RMS,  RMIH,  RMDH, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,
        _______,       RMIS,  RMDS,  RMIV,  RMDV, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,            _______, _______, _______, _______, _______, NK_TOGG, _______, _______, _______, KC_SLSH,        KC_QUES, _______,
        _______,   GUI_TOG,   _______,                      _______,                              _______, _______, _______, _______, _______
    ),
     [2] = LAYOUT(
        _______, _______,  _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,  _______, _______, _______,
        _______,    _______,_______,_______,_______,_______,_______,_______,_______, _______, _______, _______, _______, _______,
        _______,       _______,_______,_______,_______,_______,_______,_______,_______, _______, _______, _______, _______,
        _______,            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,        _______, _______,
        _______,   _______,   _______,                      _______,                              _______, _______, _______, _______, _______
    ),
     [3] = LAYOUT(
         _______, _______,  _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,  _______, _______, _______,
        _______,    _______,_______,_______,_______,_______,_______,_______,_______, _______, _______, _______, _______, _______,
        _______,       _______,_______,_______,_______,_______,_______,_______,_______, _______, _______, _______, _______,
        _______,            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,        _______, _______,
        _______,   _______,   _______,                      _______,                              _______, _______, _______, _______, _______
    ),

};

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
                                                                // Row, Column
            tap_code(dynamic_keymap_get_keycode(biton32(layer_state),4,4));
        } else {
            tap_code(dynamic_keymap_get_keycode(biton32(layer_state),4,3));
        }
    }
    return false;
}
#endif

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // Per layer indicator
    for (uint8_t i = led_min; i <= led_max; i++) {
        switch(get_highest_layer(layer_state|default_layer_state)) {
            case 0:
                rgb_matrix_set_color(i, RGB_CYAN);    // Layer 0 indicator color
                break;
            case 1:
                rgb_matrix_set_color(i, RGB_PURPLE);    // Layer 1 indicator color
                break;
            case 2:
                rgb_matrix_set_color(i, RGB_BLUE);     // Layer 2 indicator color
                break;
            case 3:
                rgb_matrix_set_color(i, RGB_ORANGE);    // Layer 3 indicator color
                break;
            default:
                break;
        }
    }

    // Layer indicator code
    //if (get_highest_layer(layer_state) > 0) {
        //uint8_t layer = get_highest_layer(layer_state);

        // Per configured key indicator
        //for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
          //  for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            //    uint8_t index = g_led_config.matrix_co[row][col];

                // Comparing LED index to matrix for configured keys or override keys
                //if (index >= led_min && index <= led_max && index != NO_LED) {

                    //uint16_t keycheck = keymap_key_to_keycode(layer, (keypos_t){col,row});
                    //HSV hsv = rgb_matrix_config.hsv;
                    //hsv.s   = 255;  // Ensure RGB colors are full saturation regardless of user's setting

                    // If key is configured, light it up
                    //if (keycheck > KC_TRNS) {

                        // Per key overrides
                        //switch (keycheck) {
                            //case RESET:
                                //hsv.h = 0;  // RED
                              //  break;
                            //default:
                                // Per key override with toggle indicator
                                //if (keycheck == NK_TOGG && keymap_config.nkro == 1) {
                                    //if (keymap_config.nkro == 1) {
                                     //   hsv.h = 85; // COLOR IF NKRO IS ENABLED
                                   // } else {
                                    //    hsv.h = 0; // COLOR IF NKRO IS DISABLED
                                  //  }
                                //} else if (keycheck == GUI_TOG) {
                                  //  if (keymap_config.no_gui == 1) {
                                //        hsv.h = 0;  // COLOR IF GUI IS DISABLED
                              //      } else {
                            //            hsv.h = 85;  // COLOR IF GUI IS ENABLED
                          //          }
                        //        }
                      //  }
                    //}

                    // Make the key lights a bit brighter
                    //if (hsv.v < (RGB_MATRIX_MAXIMUM_BRIGHTNESS - 30)) { hsv.v += 30; }
                    //else { hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS; }

                    //RGB rgb = hsv_to_rgb(hsv);
                    //RGB_MATRIX_INDICATOR_SET_COLOR(index, rgb.r, rgb.g, rgb.b);
          //      } // End of comparison code
        //    }
      //  } // End of per configured key indicator

    //} // End of layer indicator code


}
