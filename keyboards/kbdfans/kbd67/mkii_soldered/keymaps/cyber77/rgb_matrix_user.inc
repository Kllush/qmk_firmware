RGB_MATRIX_EFFECT(DIAGONAL)
RGB_MATRIX_EFFECT(COOL_DIAGONAL)
RGB_MATRIX_EFFECT(HUE_WAVE2)
RGB_MATRIX_EFFECT(HUE_PENDULUM2)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// RGB effects from HorrorTroll <https://github.com/HorrorTroll
static HSV COOL_DIAGONAL_math(HSV hsv, uint8_t i, uint8_t time) {
    hsv.h = (g_led_config.point[i].x / 4) - g_led_config.point[i].y - time;
    return hsv;
}

bool COOL_DIAGONAL(effect_params_t* params) { return effect_runner_i(params, &COOL_DIAGONAL_math); }

static HSV DIAGONAL_math(HSV hsv, uint8_t i, uint8_t time) {
    hsv.h = g_led_config.point[i].x - g_led_config.point[i].y - time;
    return hsv;
}

bool DIAGONAL(effect_params_t* params) { return effect_runner_i(params, &DIAGONAL_math); }
// End RGB effects from HoorTroll


// Hue wave effect but with a larger huedelta
static HSV HUE_WAVE2_math(HSV hsv, uint8_t i, uint8_t time) {
    uint8_t huedelta = 50;
    hsv.h            = hsv.h + scale8(abs8(g_led_config.point[i].x - time), huedelta);
    return hsv;
}

bool HUE_WAVE2(effect_params_t* params) { return effect_runner_i(params, &HUE_WAVE2_math); }

// Hue pendulum but with a larger huedelta
static HSV HUE_PENDULUM2_math(HSV hsv, uint8_t i, uint8_t time) {
    uint8_t huedelta = 50;
    hsv.h            = hsv.h + scale8(abs8(sin8(time) + (g_led_config.point[i].x) - 128), huedelta);
    return hsv;
}

bool HUE_PENDULUM2(effect_params_t* params) { return effect_runner_i(params, &HUE_PENDULUM2_math); }


#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
