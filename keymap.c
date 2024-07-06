/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

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

// "Quad Function Tap-Dance"
// https://docs.qmk.fm/features/tap_dance#example-4
// triple tap removed
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP // send two single taps
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

enum {
    A_LGUI_Ä,
    U_Ü
};

td_state_t cur_dance(tap_dance_state_t *state);

void a_finished(tap_dance_state_t *state, void *user_data);
void a_reset(tap_dance_state_t *state, void *user_data);

void u_finished(tap_dance_state_t *state, void *user_data);
void u_reset(tap_dance_state_t *state, void *user_data);


// layer declaration
enum layers{
  _QWERTZ_ZY = 0,   // quertz layout (with z and y swapped) (default)
  _COLEMAK_DH,      // colemak-dh layout
  _SYMBOL,          // symbol layout
  _FUNCTION,
  _ADJUST
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // layouts
    [_QWERTZ_ZY] = LAYOUT_split_3x6_3(
        KC_TAB,     KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,       KC_Z,   TD(U_Ü),        KC_I,               KC_O,           KC_P,               KC_BSPC,
        KC_LSFT,    TD(A_LGUI_Ä),   LALT_T(KC_S),   LSFT_T(KC_D),   LCTL_T(KC_F),   KC_G,       KC_H,   RCTL_T(KC_J),   RSFT_T(KC_K),       LALT_T(KC_L),   RGUI_T(KC_SCLN),    KC_DEL,
        KC_LCTL,    KC_Y,           KC_X,           KC_C,           KC_V,           KC_B,       KC_N,   KC_M,           KC_COMM,            KC_DOT,         KC_SLASH,           KC_ESC,
        KC_LALT,    MO(_SYMBOL),    KC_SPC,                                                     KC_ENT, MO(_FUNCTION),  KC_RALT
    ),

    [_COLEMAK_DH] = LAYOUT_split_3x6_3(
        KC_TAB,     KC_Q,           KC_W,           KC_F,           KC_P,           KC_B,       KC_J,   KC_L,           KC_U,           KC_Y,           KC_SCLN,        KC_BSPC,
        KC_LSFT,    LGUI_T(KC_A),   LALT_T(KC_R),   LSFT_T(KC_S),   LCTL_T(KC_T),   KC_G,       KC_M,   RCTL_T(KC_N),   RSFT_T(KC_E),   LALT_T(KC_I),   RGUI_T(KC_O),   KC_DEL,
        KC_LCTL,    KC_Z,           KC_X,           KC_C,           KC_D,           KC_V,       KC_K,   KC_H,           KC_COMM,        KC_DOT,         KC_SLASH,       KC_ESC,
        KC_LALT,    MO(_SYMBOL),    KC_SPC,                                                     KC_ENT, MO(_FUNCTION),  KC_RALT
    ),

    // WARNING! I use a german layout, the behavior of the keys may be different on your keyboard
    [_SYMBOL] = LAYOUT_split_3x6_3(
        KC_ESC,     KC_GRV,             S(KC_GRV),          KC_NUBS,            S(KC_NUBS),     RALT(KC_NUBS),      KC_NO,          RALT(KC_7),     RALT(KC_8),     RALT(KC_9),     RALT(KC_0),     KC_BSPC,
        KC_LSFT,    LGUI_T(KC_1),       LALT_T(KC_2),       LSFT_T(KC_3),       LCTL_T(KC_4),   KC_5,               KC_6,           RSFT_T(KC_7),   RSFT_T(KC_8),   LALT_T(KC_9),   RGUI_T(KC_0),   KC_DEL,
        KC_LCTL,    KC_MINS,            KC_EQL,             S(KC_EQL),          S(KC_MINS),     RALT(KC_MINS),      RALT(KC_RBRC),  KC_RBRC,        S(KC_RBRC),     KC_NUHS,        S(KC_NUHS),     KC_ESC,
        KC_LALT,      _______,          KC_SPC,                                                                     KC_ENT,         MO(_ADJUST),    KC_RALT
    ),

    [_FUNCTION] = LAYOUT_split_3x6_3(
        KC_TAB,     KC_F12,     	KC_F11,         KC_F10,         KC_F9,          KC_NO,          KC_PSCR,    KC_HOME,    KC_END,     KC_INS,     KC_DEL,     KC_BSPC,
        KC_LSFT,    LGUI_T(KC_F8),  LALT_T(KC_F7),  LSFT_T(KC_F6),  LCTL_T(KC_F5),  KC_NO,          KC_LEFT,    KC_DOWN,    KC_UP,      KC_RIGHT,   KC_SCRL,    KC_DEL,
        KC_LCTL,    KC_F4,      	KC_F3,          KC_F2,          KC_F1,          KC_NO,          KC_MUTE,    KC_PGDN,    KC_PGUP,    KC_MPLY,    KC_PAUS,    KC_ESC,
        KC_LALT,    MO(_ADJUST),    KC_SPC,                                                         KC_ENT,     _______,    KC_RALT
    ),

    [_ADJUST] = LAYOUT_split_3x6_3(
        DF(_QWERTZ_ZY), DF(_COLEMAK_DH),    KC_NO,      KC_NO,      QK_BOOT,    RGB_TOG,        KC_NO,      KC_NO,      KC_NO,  KC_NO, KC_NO, KC_NO,
        RGB_MOD,        RGB_HUI,            RGB_SAI,    RGB_VAI,    KC_NO,      KC_NO,          KC_NO,      KC_NO,      KC_NO,  KC_NO, KC_NO, KC_NO,
        RGB_RMOD,       RGB_HUD,            RGB_SAD,    RGB_VAD,    KC_NO,      KC_NO,          KC_NO,      KC_NO,      KC_NO,  KC_NO, KC_NO, KC_NO,
        KC_LALT,        _______,            KC_SPC,                                             KC_ENT,     _______,    KC_RALT
  )
};

// tap dance definitions
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }
    return TD_UNKNOWN;
}

static td_tap_t atap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void a_finished(tap_dance_state_t *state, void *user_data) {
    atap_state.state = cur_dance(state);
    switch (atap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_A); break;
        case TD_SINGLE_HOLD: register_code(KC_LGUI); break;
        case TD_DOUBLE_TAP: register_code(KC_QUOT); break; // "KC_Ä"
        case TD_DOUBLE_SINGLE_TAP: tap_code(KC_A); register_code(KC_A); break;
        default: break;
    }
}

void a_reset(tap_dance_state_t *state, void *user_data) {
    switch (atap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_A); break;
        case TD_SINGLE_HOLD: unregister_code(KC_LGUI); break;
        case TD_DOUBLE_TAP: unregister_code(KC_QUOT); break;
        case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_A); break;
        default: break;
    }
    atap_state.state = TD_NONE;
}

static td_tap_t utap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void u_finished(tap_dance_state_t *state, void *user_data) {
    utap_state.state = cur_dance(state);
    switch (utap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_U); break;
        case TD_SINGLE_HOLD: register_code(KC_U); break;
        case TD_DOUBLE_TAP: register_code(KC_LBRC); break; // "KC_Ü"
        case TD_DOUBLE_SINGLE_TAP: tap_code(KC_U); register_code(KC_U); break;
        default: break;
    }
}

void u_reset(tap_dance_state_t *state, void *user_data) {
    switch (utap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_U); break;
        case TD_SINGLE_HOLD: register_code(KC_U); break;
        case TD_DOUBLE_TAP: unregister_code(KC_LBRC); break;
        case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_U); break;
        default: break;
    }
    utap_state.state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
    [A_LGUI_Ä] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, a_finished, a_reset),
    [U_Ü] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, u_finished, u_reset)
};
