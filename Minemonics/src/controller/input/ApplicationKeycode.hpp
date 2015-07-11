#ifndef CONTROLLER_INPUT_APPLICATIONKEYCODE_H_
#define CONTROLLER_INPUT_APPLICATIONKEYCODE_H_

//# corresponding header
#include <controller/input/ApplicationScancode.hpp>

//# forward declarations
//# system headers
//## controller headers
//## model headers
//## view headers
//# custom headers
//## base headers
//## configuration headers
//## controller headers
//## model headers
//## view headers
//## utils headers

#define APPK_SCANCODE_MASK (1<<30)
#define APP_SCANCODE_TO_KEYCODE(X)  (X | APPK_SCANCODE_MASK)

/**
 * @brief		The APP virtual key representation defines constants which identify keyboard keys and modifiers.
 * @details		Values of this type are used to represent keyboard keys using the current
 *  layout of the keyboard.  These values include Unicode values representing
 *  the unmodified character that would be generated by pressing the key, or
 *  an APPK_* constant for those keys that do not generate characters.
 * @date		2015-02-27
 * @author		Benjamin Ellenberger
 */
class ApplicationKeycode {

public:

	enum Keycode {
		APPK_UNKNOWN = 0,

		APPK_RETURN = '\r',
		APPK_ESCAPE = '\033',
		APPK_BACKSPACE = '\b',
		APPK_TAB = '\t',
		APPK_SPACE = ' ',
		APPK_EXCLAIM = '!',
		APPK_QUOTEDBL = '"',
		APPK_HASH = '#',
		APPK_PERCENT = '%',
		APPK_DOLLAR = '$',
		APPK_AMPERSAND = '&',
		APPK_QUOTE = '\'',
		APPK_LEFTPAREN = '(',
		APPK_RIGHTPAREN = ')',
		APPK_ASTERISK = '*',
		APPK_PLUS = '+',
		APPK_COMMA = ',',
		APPK_MINUS = '-',
		APPK_PERIOD = '.',
		APPK_SLASH = '/',
		APPK_0 = '0',
		APPK_1 = '1',
		APPK_2 = '2',
		APPK_3 = '3',
		APPK_4 = '4',
		APPK_5 = '5',
		APPK_6 = '6',
		APPK_7 = '7',
		APPK_8 = '8',
		APPK_9 = '9',
		APPK_COLON = ':',
		APPK_SEMICOLON = ';',
		APPK_LESS = '<',
		APPK_EQUALS = '=',
		APPK_GREATER = '>',
		APPK_QUESTION = '?',
		APPK_AT = '@',
		/*
		 Skip uppercase letters
		 */
		APPK_LEFTBRACKET = '[',
		APPK_BACKSLASH = '\\',
		APPK_RIGHTBRACKET = ']',
		APPK_CARET = '^',
		APPK_UNDERSCORE = '_',
		APPK_BACKQUOTE = '`',
		APPK_a = 'a',
		APPK_b = 'b',
		APPK_c = 'c',
		APPK_d = 'd',
		APPK_e = 'e',
		APPK_f = 'f',
		APPK_g = 'g',
		APPK_h = 'h',
		APPK_i = 'i',
		APPK_j = 'j',
		APPK_k = 'k',
		APPK_l = 'l',
		APPK_m = 'm',
		APPK_n = 'n',
		APPK_o = 'o',
		APPK_p = 'p',
		APPK_q = 'q',
		APPK_r = 'r',
		APPK_s = 's',
		APPK_t = 't',
		APPK_u = 'u',
		APPK_v = 'v',
		APPK_w = 'w',
		APPK_x = 'x',
		APPK_y = 'y',
		APPK_z = 'z',

		APPK_CAPSLOCK = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_CAPSLOCK),

		APPK_F1 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F1),
		APPK_F2 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F2),
		APPK_F3 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F3),
		APPK_F4 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F4),
		APPK_F5 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F5),
		APPK_F6 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F6),
		APPK_F7 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F7),
		APPK_F8 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F8),
		APPK_F9 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F9),
		APPK_F10 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F10),
		APPK_F11 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F11),
		APPK_F12 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F12),

		APPK_PRINTSCREEN = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_PRINTSCREEN),
		APPK_SCROLLLOCK = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_SCROLLLOCK),
		APPK_PAUSE = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_PAUSE),
		APPK_INSERT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_INSERT),
		APPK_HOME = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_HOME),
		APPK_PAGEUP = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_PAGEUP),
		APPK_DELETE = '\177',
		APPK_END = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_END),
		APPK_PAGEDOWN = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_PAGEDOWN),
		APPK_RIGHT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_RIGHT),
		APPK_LEFT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_LEFT),
		APPK_DOWN = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_DOWN),
		APPK_UP = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_UP),

		APPK_NUMLOCKCLEAR = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_NUMLOCKCLEAR),
		APPK_KP_DIVIDE = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_DIVIDE),
		APPK_KP_MULTIPLY = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_MULTIPLY),
		APPK_KP_MINUS = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_MINUS),
		APPK_KP_PLUS = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_PLUS),
		APPK_KP_ENTER = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_ENTER),
		APPK_KP_1 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_1),
		APPK_KP_2 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_2),
		APPK_KP_3 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_3),
		APPK_KP_4 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_4),
		APPK_KP_5 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_5),
		APPK_KP_6 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_6),
		APPK_KP_7 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_7),
		APPK_KP_8 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_8),
		APPK_KP_9 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_9),
		APPK_KP_0 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_0),
		APPK_KP_PERIOD = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_PERIOD),

		APPK_APPLICATION = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_APPLICATION),
		APPK_POWER = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_POWER),
		APPK_KP_EQUALS = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_EQUALS),
		APPK_F13 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F13),
		APPK_F14 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F14),
		APPK_F15 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F15),
		APPK_F16 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F16),
		APPK_F17 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F17),
		APPK_F18 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F18),
		APPK_F19 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F19),
		APPK_F20 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F20),
		APPK_F21 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F21),
		APPK_F22 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F22),
		APPK_F23 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F23),
		APPK_F24 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_F24),
		APPK_EXECUTE = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_EXECUTE),
		APPK_HELP = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_HELP),
		APPK_MENU = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_MENU),
		APPK_SELECT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_SELECT),
		APPK_STOP = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_STOP),
		APPK_AGAIN = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_AGAIN),
		APPK_UNDO = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_UNDO),
		APPK_CUT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_CUT),
		APPK_COPY = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_COPY),
		APPK_PASTE = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_PASTE),
		APPK_FIND = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_FIND),
		APPK_MUTE = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_MUTE),
		APPK_VOLUMEUP = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_VOLUMEUP),
		APPK_VOLUMEDOWN = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_VOLUMEDOWN),
		APPK_KP_COMMA = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_COMMA),
		APPK_KP_EQUALSAS400 = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_KP_EQUALSAS400),

		APPK_ALTERASE = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_ALTERASE),
		APPK_SYSREQ = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_SYSREQ),
		APPK_CANCEL = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_CANCEL),
		APPK_CLEAR = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_CLEAR),
		APPK_PRIOR = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_PRIOR),
		APPK_RETURN2 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_RETURN2),
		APPK_SEPARATOR = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_SEPARATOR),
		APPK_OUT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_OUT),
		APPK_OPER = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_OPER),
		APPK_CLEARAGAIN = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_CLEARAGAIN),
		APPK_CRSEL = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_CRSEL),
		APPK_EXSEL = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_EXSEL),

		APPK_KP_00 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_00),
		APPK_KP_000 = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_000),
		APPK_THOUSANDSSEPARATOR = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_THOUSANDSSEPARATOR),
		APPK_DECIMALSEPARATOR = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_DECIMALSEPARATOR),
		APPK_CURRENCYUNIT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_CURRENCYUNIT),
		APPK_CURRENCYSUBUNIT = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_CURRENCYSUBUNIT),
		APPK_KP_LEFTPAREN = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_LEFTPAREN),
		APPK_KP_RIGHTPAREN = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_KP_RIGHTPAREN),
		APPK_KP_LEFTBRACE = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_LEFTBRACE),
		APPK_KP_RIGHTBRACE = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_KP_RIGHTBRACE),
		APPK_KP_TAB = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_TAB),
		APPK_KP_BACKSPACE = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_BACKSPACE),
		APPK_KP_A = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_A),
		APPK_KP_B = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_B),
		APPK_KP_C = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_C),
		APPK_KP_D = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_D),
		APPK_KP_E = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_E),
		APPK_KP_F = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_F),
		APPK_KP_XOR = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_XOR),
		APPK_KP_POWER = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_POWER),
		APPK_KP_PERCENT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_PERCENT),
		APPK_KP_LESS = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_LESS),
		APPK_KP_GREATER = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_GREATER),
		APPK_KP_AMPERSAND = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_AMPERSAND),
		APPK_KP_DBLAMPERSAND = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_KP_DBLAMPERSAND),
		APPK_KP_VERTICALBAR = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_KP_VERTICALBAR),
		APPK_KP_DBLVERTICALBAR = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_KP_DBLVERTICALBAR),
		APPK_KP_COLON = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_COLON),
		APPK_KP_HASH = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_HASH),
		APPK_KP_SPACE = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_SPACE),
		APPK_KP_AT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_AT),
		APPK_KP_EXCLAM = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_EXCLAM),
		APPK_KP_MEMSTORE = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_MEMSTORE),
		APPK_KP_MEMRECALL = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_MEMRECALL),
		APPK_KP_MEMCLEAR = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_MEMCLEAR),
		APPK_KP_MEMADD = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_MEMADD),
		APPK_KP_MEMSUBTRACT = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_KP_MEMSUBTRACT),
		APPK_KP_MEMMULTIPLY = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_KP_MEMMULTIPLY),
		APPK_KP_MEMDIVIDE = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_MEMDIVIDE),
		APPK_KP_PLUSMINUS = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_PLUSMINUS),
		APPK_KP_CLEAR = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_CLEAR),
		APPK_KP_CLEARENTRY = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_KP_CLEARENTRY),
		APPK_KP_BINARY = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_BINARY),
		APPK_KP_OCTAL = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_OCTAL),
		APPK_KP_DECIMAL = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KP_DECIMAL),
		APPK_KP_HEXADECIMAL = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_KP_HEXADECIMAL),

		APPK_LCTRL = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_LCTRL),
		APPK_LSHIFT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_LSHIFT),
		APPK_LALT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_LALT),
		APPK_LGUI = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_LGUI),
		APPK_RCTRL = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_RCTRL),
		APPK_RSHIFT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_RSHIFT),
		APPK_RALT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_RALT),
		APPK_RGUI = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_RGUI),

		APPK_MODE = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_MODE),

		APPK_AUDIONEXT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_AUDIONEXT),
		APPK_AUDIOPREV = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_AUDIOPREV),
		APPK_AUDIOSTOP = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_AUDIOSTOP),
		APPK_AUDIOPLAY = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_AUDIOPLAY),
		APPK_AUDIOMUTE = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_AUDIOMUTE),
		APPK_MEDIASELECT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_MEDIASELECT),
		APPK_WWW = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_WWW),
		APPK_MAIL = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_MAIL),
		APPK_CALCULATOR = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_CALCULATOR),
		APPK_COMPUTER = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_COMPUTER),
		APPK_AC_SEARCH = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_AC_SEARCH),
		APPK_AC_HOME = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_AC_HOME),
		APPK_AC_BACK = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_AC_BACK),
		APPK_AC_FORWARD = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_AC_FORWARD),
		APPK_AC_STOP = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_AC_STOP),
		APPK_AC_REFRESH = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_AC_REFRESH),
		APPK_AC_BOOKMARKS = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_AC_BOOKMARKS),

		APPK_BRIGHTNESSDOWN = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_BRIGHTNESSDOWN),
		APPK_BRIGHTNESSUP = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_BRIGHTNESSUP),
		APPK_DISPLAYSWITCH = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_DISPLAYSWITCH),
		APPK_KBDILLUMTOGGLE = APP_SCANCODE_TO_KEYCODE(
				APP_SCANCODE_KBDILLUMTOGGLE),
		APPK_KBDILLUMDOWN = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KBDILLUMDOWN),
		APPK_KBDILLUMUP = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_KBDILLUMUP),
		APPK_EJECT = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_EJECT),
		APPK_SLEEP = APP_SCANCODE_TO_KEYCODE(APP_SCANCODE_SLEEP)
	};

	/**
	 * \brief Enumeration of valid key mods (possibly OR'd together).
	 */
	typedef enum {
		KMOD_NONE = 0x0000,
		KMOD_LSHIFT = 0x0001,
		KMOD_RSHIFT = 0x0002,
		KMOD_LCTRL = 0x0040,
		KMOD_RCTRL = 0x0080,
		KMOD_LALT = 0x0100,
		KMOD_RALT = 0x0200,
		KMOD_LGUI = 0x0400,
		KMOD_RGUI = 0x0800,
		KMOD_NUM = 0x1000,
		KMOD_CAPS = 0x2000,
		KMOD_MODE = 0x4000,
		KMOD_RESERVED = 0x8000
	} APP_Keymod;

#define KMOD_CTRL   (KMOD_LCTRL|KMOD_RCTRL)
#define KMOD_SHIFT  (KMOD_LSHIFT|KMOD_RSHIFT)
#define KMOD_ALT    (KMOD_LALT|KMOD_RALT)
#define KMOD_GUI    (KMOD_LGUI|KMOD_RGUI)

};
/* vi: set ts=4 sw=4 expandtab: */

#endif /* CONTROLLER_INPUT_APPLICATIONKEYCODE_H_ */
