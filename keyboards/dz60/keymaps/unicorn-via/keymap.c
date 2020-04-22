#include QMK_KEYBOARD_H

enum keyboard_layers {
  _BASE = 0, 	// Base Layer
  _FUNC,     	// Function Layer
  _RGB,     	// RGB control Layer
  _SYS			  // System control layer (reset, sleep, etc)
};

enum my_keycodes {
  KC_00 = SAFE_RANGE
};


  /* RGBLIGHT strip wiring. LED #75 to #99 is for underglow
  *  NEED TO REWRITE SECTION
  * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
  * │0  │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 10│ 11│ 12│ 13    │
  * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
  * │ 14  │15 │16 │17 │18 │19 │20 │21 │22 │23 │24 │25 │26 │27   │
  * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤
  * │ 28   │29 │30 │31 │32 │33 │34 │35 │36 │37 │38 │39 │ 40     │
  * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┤
  * │ 41     │42 │43 │44 │45 │46 │47 │48 │49 │50 │51 │52    │53 │
  * ├────┬───┴┬──┴─┬─┴───┴───┴┬──┴─┬─┴───┴──┬┴──┬┴──┬┴──┬───┼───┤
  * │ 54 │ 55 │ 56 │  57      │ 58 │  59    │60 │61 │62 │63 │64 │
  * └────┴────┴────┴──────────┴────┴────────┴───┴───┴───┴───┴───┘
  */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Base layer / keyboard layout (FN2 = scroll lock when pressed)
  *  Backspace & left shift is full size compared to default layout
  *  2.75u * 2.25u space bars are swapped (longer one on left hand side)
  * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
  * │esc│ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 10│ 11│ 12│ bkspc │
  * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
  * │tab  │ q │ w │ e │ r │ t │ y │ u │ i │ o │ p │ [ │ ] │ \   │
  * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤
  * │caps  │ a │ s │ d │ f │ g │ h │ j │ k │ l │ ; │ ' │ enter  │
  * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┤
  * │  shift │ z │ x │ c │ v │ b │ n │ m │ , │ . │ / │shift │ ↑ │
  * ├────┬───┴┬──┴─┬─┴───┴───┴┬──┴─┬─┴───┴──┬┴──┬┴──┬┴──┬───┼───┤
  * │ctrl│win │alt │          │ FN │        │FN2│FN │ ← │ ↓ │ → │
  * └────┴────┴────┴──────────┴────┴────────┴───┴───┴───┴───┴───┘
  */
  [_BASE] = LAYOUT(
    LT(_SYS, KC_ESC), KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, _______, KC_BSPC,
    KC_TAB , KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
    KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,
    KC_LSFT, _______, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP,
    KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, MO(_FUNC), KC_SPC, LT(_RGB, KC_SLCK), MO(_FUNC), KC_LEFT, KC_DOWN, KC_RIGHT
    ),
  /* function layer - NEED TO ADD INSERT KEYCODE
  * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
  * │~  │F1 │F2 │F3 │F4 │F5 │F6 │F7 │F8 │F9 │F10│F11│F12│delete │
  * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
  * │     │   │   │   │   │   │   │ 7 │ 8 │ 9 │   │   │   │     │
  * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤
  * │      │   │   │   │   │   │   │ 4 │ 5 │ 6 │   │   │ insert │
  * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┤
  * │        │   │   │   │   │   │   │ 1 │ 2 │ 3 │   │NLCK  │pup│
  * ├────┬───┴┬──┴─┬─┴───┴───┴┬──┴─┬─┴───┴──┬┴──┬┴──┬┴──┬───┼───┤
  * │    │    │    │          │    │  0     │   │   │hom│pdn│end│
  * └────┴────┴────┴──────────┴────┴────────┴───┴───┴───┴───┴───┘
  */
  [_FUNC] = LAYOUT(
    KC_TILD, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, _______, KC_DEL,
    _______, _______, _______, _______, _______, _______, _______, KC_P7, KC_P8, KC_P9, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, KC_P4, KC_P5, KC_P6, _______, _______, KC_INS,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_P1, KC_P2, KC_P3, _______, KC_NLCK, KC_PGUP,
    _______, _______, _______, _______, _______, KC_P0, _______, _______, KC_HOME, KC_PGDN, KC_END
    ),
  /* RGB layer (RMD = RGB_RMOD, MSW = RGB_M_SW)
  * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
  * │   │   │   │   │   │   │   │   │   │   │   │   │   │wininfo│
  * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
  * │     │   │   │   │VAD│VAI│   │   │HUI│   │psc│   │   │pscr │
  * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤
  * │      │   │   │   │TOG│MOD│RMD│SAD│HUD│SAI│   │   │        │
  * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┤
  * │        │   │   │   │MSW│SPI│SPD│   │   │   │   │      │vup│
  * ├────┬───┴┬──┴─┬─┴───┴───┴┬──┴─┬─┴───┴──┬┴──┬┴──┬┴──┬───┼───┤
  * │    │    │    │          │    │        │   │mut│ply│ply│vdn│
  * └────┴────┴────┴──────────┴────┴────────┴───┴───┴───┴───┴───┘
  */
  [_RGB] = LAYOUT(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, LGUI(KC_PAUS),
    _______, _______, _______, _______, RGB_VAD, RGB_VAI, _______, _______, RGB_HUI, _______, LALT(KC_PSCR), _______, _______, LGUI(KC_PSCR),
    _______, _______, _______, _______, RGB_TOG, RGB_MOD, RGB_RMOD, RGB_SAD, RGB_HUD, RGB_SAI, _______, _______, _______,
    _______, _______, _______, _______, _______, RGB_M_SW, RGB_SPI, RGB_SPD, _______, _______, _______, _______, _______, KC_VOLU,
    _______, _______, _______, _______, _______, _______, _______, KC_MUTE, KC_MPLY, KC_MPLY, KC_VOLD
    ),
  /* System layer
  * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
  * │   │   │   │   │   │   │   │   │   │   │   │   │   │sleep  │
  * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
  * │     │   │   │   │   │   │   │   │   │   │   │   │   │     │
  * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤
  * │Reset │   │   │   │   │   │   │   │   │   │   │   │power   │
  * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┤
  * │        │   │   │   │   │   │   │   │   │   │   │      │   │
  * ├────┬───┴┬──┴─┬─┴───┴───┴┬──┴─┬─┴───┴──┬┴──┬┴──┬┴──┬───┼───┤
  * │    │    │    │          │    │        │   │   │   │   │   │
  * └────┴────┴────┴──────────┴────┴────────┴───┴───┴───┴───┴───┘
  */
  [_SYS] = LAYOUT(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_SLEP,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    RESET  , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_PWR,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
};
