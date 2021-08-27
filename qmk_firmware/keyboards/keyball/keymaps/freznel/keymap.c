/*
Copyright 2021 @Yowkees

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "pointing_device.h"
#include "oledkit.h"

enum keymap_layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _BALL,
};

enum custom_keycodes {
    KC_QWERTY = SAFE_RANGE,
    KC_COLEMAK,
    KC_LOWER,
    KC_RAISE,
    KC_ADJUST,
    KC_PRVWD,
    KC_NXTWD,
    KC_LSTRT,
    KC_LEND,
    KC_DLINE,
    BALL_SCR,//scrolls
    BALL_NCL,//left click
    BALL_RCL,//right click
    BALL_MCL,//middle click
    KC_REDO,
};

/////////////////////////////////////////////////////////////////////////////
// Special key codes for shorthand

// clang-format off

// layer
#define KC_L_SPC    LT(_LOWER, KC_SPC)
#define KC_R_ENT    LT(_RAISE, KC_ENT)

// shift_t
#define KC_S_EN     LSFT_T(KC_LANG2)

// original
#define KC_A_JA     LT(_BALL, KC_LANG1)     // cmd or adjust
#define KC_AL_CP    MT(MOD_LALT, KC_CAPS)   // alt or caps lock
#define KC_G_BS     MT(MOD_LGUI, KC_BSPC)   // command or back space
#define KC_G_DEL    MT(MOD_LGUI, KC_DEL)    // command or delete
#define KC_A_BS     LT(_BALL, KC_BSPC)      // adjust or back space
#define KC_A_DEL    LT(_BALL, KC_DEL)       // adjust or delete

#define RBR_RGU MT(MOD_RGUI, KC_RBRC)
#define F12_RGU MT(MOD_RGUI, KC_F12)
#define PLS_LCT MT(MOD_LCTL, KC_PPLS)
#define EQL_LCT MT(MOD_LCTL, KC_PEQL)
#define APP_LCT MT(MOD_LCTL, KC_APP)
#define EQL_RCT MT(MOD_RCTL, KC_PEQL)
#define QUO_RCT MT(MOD_RCTL, KC_QUOT)
#define APP_RCT MT(MOD_RCTL, KC_APP)
#define MIN_RCT MT(MOD_RCTL, KC_MINS)
#define EQL_LAL MT(MOD_LALT, KC_EQL)
#define BSL_RAL MT(MOD_RALT, KC_BSLS)

#define NBS_LCT MT(MOD_LCTL, KC_NUBS)
#define BSH_LAL MT(MOD_LALT, KC_BSLS)
#define APP_RAL MT(MOD_RALT, KC_APP)

#define BSP_RSH MT(MOD_RSFT, KC_BSPC)
#define SPC_LSH MT(MOD_LSFT, KC_SPC)
#define DEL_RSE LT(_RAISE, KC_DEL)
#define TAB_RSE LT(_RAISE, KC_TAB)
#define ENT_LWR LT(_LOWER, KC_ENT)
#define ESC_LWR LT(_LOWER, KC_ESC)

#define S_BSPC LSFT_T(KC_BSPC)
#define R_DEL LT(_RAISE, KC_DEL)
#define G_ENT LGUI_T(KC_ENT)
#define L_SPC LT(_LOWER, KC_SPC)
// clang-format on

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_QWERTY] = LAYOUT_double_balls(
      KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                           KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
      KC_TAB,   KC_Q,   KC_W,    KC_F,    KC_P,    KC_B, BALL_NCL,                      BALL_NCL, KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN,    KC_BSLS,
      KC_ADJUST, LGUI_T(KC_A),   LALT_T(KC_R),   LCTL_T(KC_S),   LSFT_T(KC_T),    KC_G,  BALL_RCL, BALL_RCL,  KC_M,    LSFT_T(KC_N),   LCTL_T(KC_E),   LALT_T(KC_I),   LGUI_T(KC_O), RCTL_T(KC_QUOT),
      BALL_SCR, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,   TAB_RSE, KC_MUTE,      RGB_TOG, R_DEL,KC_K,    KC_H,   KC_COMM,    KC_DOT, KC_SLSH, BSL_RAL,
      KC_LCTL, KC_LALT, KC_LGUI, LALT(KC_TAB), KC_RAISE,  SPC_LSH,   ENT_LWR,    ESC_LWR,   BSP_RSH,  KC_RAISE,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
    ),

	[_LOWER] = LAYOUT_double_balls(
      _______, KC_F1, KC_F2, KC_F3, KC_F4,   KC_F5,                                KC_F6,   KC_F7,   KC_F8, KC_F9, KC_F10, KC_F11,
      KC_PSLS, KC_NLCK, KC_P7, KC_P8, KC_P9, KC_KP_PLUS, KC_MINS,                   KC_EQL, KC_UNDO, KC_COPY, KC_CUT, KC_PASTE, KC_REDO, KC_F12,
      KC_CAPS, KC_KP_ASTERISK, KC_P4, KC_P5, KC_P6, KC_KP_SLASH, KC_LBRC,                  KC_RBRC, KC_PGUP, C(S(KC_LEFT)), C(S(KC_DOWN)), C(S(KC_UP)),  C(S(KC_RIGHT)), KC_NLCK,
      _______, KC_KP_DOT, KC_P1, KC_P2, KC_P3, S_BSPC, _______, _______,  _______, _______, KC_PGDN, KC_PRVWD, KC_LSTRT, KC_LEND, KC_NXTWD, _______,
      _______, KC_LGUI,KC_P0, KC_PENT, _______,     _______,  _______,   _______,    _______,   _______, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT
      ),

	[_RAISE] = LAYOUT_double_balls(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                           KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
      KC_PSLS, KC_EXLM, KC_AT, KC_HASH,  KC_DLR, KC_PERC, BALL_MCL,                    BALL_MCL, KC_BSPC, KC_HOME, KC_UP, KC_END, KC_DEL, KC_F12,
      KC_CAPS, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, C(S(KC_T)),                    C(S(KC_W)), KC_PGUP, KC_LEFT, KC_DOWN, KC_RGHT, KC_P6, KC_NLCK,
      _______, sKC_EQL, KC_MINS, KC_PLUS, KC_LCBR, KC_RCBR, _______, _______,  _______, _______, KC_PGDN, KC_PRVWD, KC_LSTRT, KC_LEND, KC_NXTWD, _______,
      _______, KC_LALT, KC_LGUI, LALT(KC_TAB), _______,    _______,   _______,  _______,    _______,    _______, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT
      ),

	[_BALL] = LAYOUT_double_balls(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                              KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
      _______, _______, _______, _______, _______, _______, _______,                   _______, _______, EEP_RST, _______, _______, _______, KC_F12,
      _______, _______, _______, _______, _______, _______, _______,                   _______, _______, RGB_TOG, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_MOD, RGB_SPI, RGB_HUI, RGB_SAI, RGB_VAI,
      _______, _______, _______, _______, _______,      _______,     _______, _______,     _______,      RGB_RMOD, RGB_SPD, RGB_HUD, RGB_SAD, RGB_VAD
      )

};
// clang-format on

const rgblight_segment_t PROGMEM my_default[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 68, 0, 255, 80}       // Light 4 LEDs, starting with LED 6
    // {12, 4, HSV_RED}       // Light 4 LEDs, starting with LED 12
);

const rgblight_segment_t PROGMEM my_lower[] = RGBLIGHT_LAYER_SEGMENTS(
    {6, 2, 36, 255, 170},
    {8, 3, 128, 255, 170},
    {11, 4, 36, 255, 170},
    {15, 3, 128, 255, 170},
    {18, 4, 36, 255, 170},
    {22, 3, 128, 255, 170},
    {25, 1, 36, 255, 170},
    {30, 3, 128, 255, 170},
    {49, 4, 234, 128, 170},
    {55, 4 , 191, 255, 170}
);

const rgblight_segment_t PROGMEM my_raise[] = RGBLIGHT_LAYER_SEGMENTS(
    {12, 1, 0, 255, 170},
    {13, 1, 170, 255, 170},
    {47, 1, 170, 255, 170},
    {46, 1, 0, 255, 170},
    {43, 1, 85, 255, 170},
    {49, 3, 85, 255, 170},
    {55, 4 , 191, 255, 170}

);

const rgblight_segment_t PROGMEM my_ball[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 68, 0, 0, 0}
);


const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    my_default,
    my_lower,    // Overrides caps lock layer   // Overrides other layers
    my_raise,  // Overrides other layers
    my_ball
);

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
}

layer_state_t layer_state_set_user(layer_state_t state) {
	rgblight_set_layer_state(0, layer_state_cmp(state, _QWERTY));
	rgblight_set_layer_state(1, layer_state_cmp(state, _LOWER));
	rgblight_set_layer_state(2, layer_state_cmp(state, _RAISE));
  rgblight_set_layer_state(3, layer_state_cmp(state, _BALL));
  switch (get_highest_layer(state)) {
      case _BALL:
            keyball_set_scroll_mode(true);
            break;
        default:
            keyball_set_scroll_mode(false);
            break;
  }
return state;
}



#ifdef OLED_DRIVER_ENABLE

void oledkit_render_info_user(void) {
    const char *n;
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            n = PSTR("Default");
            break;
        case _RAISE:
            n = PSTR("Raise");
            break;
        case _LOWER:
            n = PSTR("Lower");
            break;
        case _BALL:
            n = PSTR("Adjust");
            break;
        default:
            n = PSTR("Undefined");
            break;
    }
    oled_write_P(PSTR("Layer: "), false);
    oled_write_ln_P(n, false);

    keyball_oled_render_ballinfo();
    keyball_oled_render_keyinfo();
}

#endif


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // case G_ENT:
        //     if (record->event.pressed) {
        //       trackball_set_scrolling(true);
        //       trackball_set_rgbw(RGB_CYAN, 0x00);
        //     } else{
        //        trackball_set_scrolling(false);
        //        trackball_set_rgbw(RGB_RED, 0x00);
        //        {
        //     return false;
        // case KC_COLEMAK:
        //     if (record->event.pressed) {
        //         set_single_persistent_default_layer(_COLEMAK);
        //     }
        //     return false;
        case KC_LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
        case KC_RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
        case KC_ADJUST:
            if (record->event.pressed) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            return false;
        case KC_PRVWD:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_LEFT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                }
            }
            break;
        case KC_NXTWD:
             if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_RIGHT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                }
            }
            break;
        case KC_LSTRT:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                     //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                } else {
                    register_code(KC_HOME);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_code(KC_HOME);
                }
            }
            break;
        case KC_LEND:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                } else {
                    register_code(KC_END);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_code(KC_END);
                }
            }
            break;
        case KC_DLINE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_BSPC);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_BSPC);
            }
            break;
        case KC_COPY:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_C);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_C);
            }
            return false;
        case KC_PASTE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_V);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_V);
            }
            return false;
        case KC_CUT:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_X);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_X);
            }
            return false;
            break;
        case KC_UNDO:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_Z);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_Z);
            }
            return false;
            break;
      case KC_REDO:
            if (record->event.pressed) {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_Y);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_Y);
                }
                return false;
                break;
     // case BALL_SCR:
        //  if(record->event.pressed){
      //        trackball_set_scrolling(true);
        //      trackball_set_rgbw(RGB_CYAN, 0x00);
        //    } else{
         //      trackball_set_scrolling(false);
        //       trackball_set_rgbw(RGB_RED, 0x00);
//}
// break;
case BALL_NCL:
   record->event.pressed?register_code(KC_BTN1):unregister_code(KC_BTN1);
   break;
case BALL_RCL:
    record->event.pressed?register_code(KC_BTN2):unregister_code(KC_BTN2);
    break;
case BALL_MCL:
    record->event.pressed?register_code(KC_BTN3):unregister_code(KC_BTN3);
    break;
}
return true;
}

#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 1) {
      switch (get_highest_layer(layer_state)) {
        case _RAISE:
            clockwise?tap_code16(C(KC_TAB)):tap_code16(S(C(KC_TAB)));
            break;
        case _LOWER:
            clockwise?tap_code(KC_WH_U):tap_code(KC_WH_D);
            break;
        default:
            clockwise?tap_code(KC_PGDN):tap_code(KC_PGUP);
            break;
      }
    }
    return true;
}
#endif
