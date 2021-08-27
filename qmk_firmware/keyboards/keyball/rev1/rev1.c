#include "rev1.h"

#include "quantum.h"
#include "pointing_device.h"

#include "trackball.h"

#ifndef KEYBALL_SCROLL_DIVIDER
#    define KEYBALL_SCROLL_DIVIDER 10
#endif
#if KEYBALL_SCROLL_DIVIDER <= 0
#    error "KEYBALL_SCROLL_DIVIDER should be larger than zero"
#endif

#ifndef KEYBALL_POINTER_DIVIDER
#    define KEYBALL_POINTER_DIVIDER 1
#endif
#if KEYBALL_POINTER_DIVIDER <= 0
#    error "KEYBALL_POINTER_DIVIDER should be larger than zero"
#endif

static int  primary        = 0;
static int  secondary      = 1;
static bool is_scroll_mode = false;

__attribute__((weak)) void pointing_device_init(void) { trackball_init(); }

// clip2int8 clips an integer fit into int8_t.
static inline int8_t clip2int8(int16_t v) { return (v) < -127 ? -127 : (v) > 127 ? 127 : (int8_t)v; }

static trackball_delta_t ball1, ball2;

__attribute__((weak)) void pointing_device_task(void) {
    trackball_delta_t d0 = {0}, d1 = {0};
    bool              c0 = trackball_consume_delta(primary, is_scroll_mode ? KEYBALL_SCROLL_DIVIDER : KEYBALL_POINTER_DIVIDER, &d0);
    bool              c1 = trackball_consume_delta(secondary, KEYBALL_SCROLL_DIVIDER, &d1);
    if (c0 || c1) {
        ball1 = d0;
        ball2 = d1;
        keyball_process_trackball_user(&d0, &d1);
    }
}

static uint16_t last_keycode;
static uint8_t  last_row;
static uint8_t  last_col;

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    last_keycode = keycode;
    last_row     = record->event.key.row;
    last_col     = record->event.key.col;
    if (!process_record_user(keycode, record)) {
        return false;
    }

    // translate KC_BTN? event to mouse report
    if (IS_MOUSEKEY_BUTTON(keycode)) {
        report_mouse_t r = {};
        r                = pointing_device_get_report();
        if (record->event.pressed) {
            r.buttons |= 1 << (keycode - KC_MS_BTN1);
        } else {
            r.buttons &= ~(1 << (keycode - KC_MS_BTN1));
        }
        pointing_device_set_report(r);
        return false;
    }
    return true;
}

#ifdef OLED_DRIVER_ENABLE

static const char *format_4d(int8_t d) {
    static char buf[5] = {0};  // max width (4) + NUL (1)
    char        lead   = ' ';
    if (d < 0) {
        d    = -d;
        lead = '-';
    }
    buf[3] = (d % 10) + '0';
    d /= 10;
    if (d == 0) {
        buf[2] = lead;
        lead   = ' ';
    } else {
        buf[2] = (d % 10) + '0';
        d /= 10;
    }
    if (d == 0) {
        buf[1] = lead;
        lead   = ' ';
    } else {
        buf[1] = (d % 10) + '0';
        d /= 10;
    }
    buf[0] = lead;
    return buf;
}

static char to_1x(uint8_t x) {
    x &= 0x0f;
    return x < 10 ? x + '0' : x + 'a' - 10;
}

#endif

//////////////////////////////////////////////////////////////////////////////
// Keyball API

bool keyball_get_scroll_mode(void) { return is_scroll_mode; }

void keyball_set_scroll_mode(bool mode) {
    if (is_scroll_mode != mode) {
        is_scroll_mode = mode;
        trackball_reset_delta(primary);
    }
}

void keyball_process_trackball_default(const trackball_delta_t *primary, const trackball_delta_t *secondary) {
    report_mouse_t r = pointing_device_get_report();
    if (primary) {
        if (!is_scroll_mode) {
            r.x += clip2int8(primary->x);
            r.y += clip2int8(primary->y);
        } else {
            r.h += clip2int8(primary->x);
            r.v -= clip2int8(primary->y);
        }
    }
    if (secondary) {
        r.h += clip2int8(secondary->x);
        r.v -= clip2int8(secondary->y);
    }
    pointing_device_set_report(r);
    pointing_device_send();
}

__attribute__((weak)) void keyball_process_trackball_user(const trackball_delta_t *primary, const trackball_delta_t *secondary) { keyball_process_trackball_default(primary, secondary); }

static bool should_swap_primary_trackball(void) {
    // TODO: support trackball handness.
    return !trackball_has() || is_keyboard_left();
}

void keyball_adjust_trackball_handness(void) {
    if (should_swap_primary_trackball()) {
        primary   = 1;
        secondary = 0;
    }
}

void keyball_oled_render_ballinfo(void) {
#ifdef OLED_DRIVER_ENABLE
    // Format: `Ball:{ball#1 x}{ball#1 y}{ball#2 x}{ball#2 y}
    //
    // Output example:
    //
    //     Ball: -12  34   0   0
    //
    oled_write_P(PSTR("Ball:"), false);
    oled_write(format_4d(ball1.x), false);
    oled_write(format_4d(ball1.y), false);
    oled_write(format_4d(ball2.x), false);
    oled_write(format_4d(ball2.y), false);
#endif
}

// clang-format off
const char PROGMEM code_to_name[] = {
    'a', 'b', 'c', 'd', 'e', 'f',  'g', 'h', 'i',  'j',
    'k', 'l', 'm', 'n', 'o', 'p',  'q', 'r', 's',  't',
    'u', 'v', 'w', 'x', 'y', 'z',  '1', '2', '3',  '4',
    '5', '6', '7', '8', '9', '0',  'R', 'E', 'B',  'T',
    '_', '-', '=', '[', ']', '\\', '#', ';', '\'', '`',
    ',', '.', '/',
};
// clang-format on

void keyball_oled_render_keyinfo(void) {
#ifdef OLED_DRIVER_ENABLE
    // Format: `Key:   R{row}  C{col} K{kc}  '{name}`
    //
    // Where `kc` is lower 8 bit of keycode.
    // Where `name` is readable label for `kc`, valid between 4 and 56.
    //
    // It is aligned to fit with output of keyball_oled_render_ballinfo().
    // For example:
    //
    //     Ball:   0   0   0   0
    //     Key:   R2  C3 K06  'c
    //
    char    name    = '\0';
    uint8_t keycode = last_keycode;
    if (keycode >= 4 && keycode < 53) {
        name = pgm_read_byte(code_to_name + keycode - 4);
    }

    oled_write_P(PSTR("Key:   R"), false);
    oled_write_char(to_1x(last_row), false);
    oled_write_P(PSTR("  C"), false);
    oled_write_char(to_1x(last_col), false);
    if (keycode) {
        oled_write_P(PSTR(" K"), false);
        oled_write_char(to_1x(keycode >> 4), false);
        oled_write_char(to_1x(keycode), false);
        // oled_write(format_02x((uint8_t)keycode), false);
    }
    if (name) {
        oled_write_P(PSTR("  '"), false);
        oled_write_char(name, false);
    }
#endif
}

#ifdef RGB_MATRIX_ENABLE
led_config_t g_led_config = { {
// Key Matrix to LED Index
  // Left Hand
  { 5, 4, 3, 2, 1, 0, 12 },
  { 6, 7, 8, 9, 10, 11, 13 },
  { 19, 18, 17, 16, 15, 14, 26 },
  { 20, 21, 22, 23, 24, 25, NO_LED },
  { 33, 32, 31, 30, 29, 28, 27},
  // Right Hand
  { 39, 38, 37, 36, 35, 34, 46 },
  { 40, 41, 42, 43, 44, 45, 47 },
  { 53, 52, 51, 50, 49, 48, 60 },
  { 54, 55, 56, 57, 58, 59, NO_LED },
  { 67, 66, 65, 64, 63, 62, 61 },

  
}, {
  // LED Index to Physical Position
  // Left Hand
  { 93,  0 }, { 74,  0 }, { 56,  0 }, { 37,  0 }, { 19,  0 }, { 0,  0 }, { 0,  16 },
  { 19,  16 }, { 37,  16 }, { 56,  16 }, { 74,  16 }, { 93,  16 }, { 112,  24 }, { 112,  40 },
  { 93,  32 }, { 74,  32 }, { 56,  32 }, { 37,  32 }, { 19,  32 }, { 0,  32 }, { 0,  48 },
  { 19,  48 }, { 37,  48 }, { 56,  48 }, { 74,  48 }, { 93,  48 }, { 112,  52 },
  { 112,  64 }, { 93,  64 }, { 74,  64 }, { 56,  64 }, { 37,  64 }, { 19,  64 }, { 0,  64 },
  // Right Hand
  { 132,  0 }, { 150,  0 }, { 169,  0 }, { 187,  0 }, { 206,  0 }, { 224,  0 }, { 224,  16 },
  { 206,  16 }, { 187,  16 }, { 169,  16 }, { 150,  16 }, { 132,  16 }, { 113,  24 }, { 113,  40 },
  { 132,  32 }, { 150,  32 }, { 169,  32 }, { 187,  32 }, { 206,  32 }, { 224,  32 }, { 224,  48 },
  { 206,  48 }, { 187,  48 }, { 169,  48 }, { 150,  48 }, { 132,  48 }, { 113,  52 },
  { 113,  64 }, { 132,  64 }, { 150,  64 }, { 169,  64 }, { 187,  64 }, { 206,  64 }, { 224,  64 },
}, {
  // LED Index to Flag
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
} };
#endif
