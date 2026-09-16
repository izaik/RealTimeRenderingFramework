#pragma once

//Not all scancodes are implemented, only commonly used keys
//We remap keys from Win32 (IBM standard layout) to 0x07 (USB interfaces)
//This is done exactly how SDL does it, we just drop SDL_ to be less confusing
//https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input#scan-codes
//https://wiki.libsdl.org/SDL3/SDL_Scancode
//https://github.com/libsdl-org/SDL/blob/main/src/events/scancodes_windows.h

typedef enum Scancode
{
    SCANCODE_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
     /* @{ */

    SCANCODE_A = 4,
    SCANCODE_B = 5,
    SCANCODE_C = 6,
    SCANCODE_D = 7,
    SCANCODE_E = 8,
    SCANCODE_F = 9,
    SCANCODE_G = 10,
    SCANCODE_H = 11,
    SCANCODE_I = 12,
    SCANCODE_J = 13,
    SCANCODE_K = 14,
    SCANCODE_L = 15,
    SCANCODE_M = 16,
    SCANCODE_N = 17,
    SCANCODE_O = 18,
    SCANCODE_P = 19,
    SCANCODE_Q = 20,
    SCANCODE_R = 21,
    SCANCODE_S = 22,
    SCANCODE_T = 23,
    SCANCODE_U = 24,
    SCANCODE_V = 25,
    SCANCODE_W = 26,
    SCANCODE_X = 27,
    SCANCODE_Y = 28,
    SCANCODE_Z = 29,

    SCANCODE_1 = 30,
    SCANCODE_2 = 31,
    SCANCODE_3 = 32,
    SCANCODE_4 = 33,
    SCANCODE_5 = 34,
    SCANCODE_6 = 35,
    SCANCODE_7 = 36,
    SCANCODE_8 = 37,
    SCANCODE_9 = 38,
    SCANCODE_0 = 39,

    SCANCODE_RETURN = 40,
    SCANCODE_ESCAPE = 41,
    SCANCODE_BACKSPACE = 42,
    SCANCODE_TAB = 43,
    SCANCODE_SPACE = 44,

    SCANCODE_MINUS = 45,
    SCANCODE_EQUALS = 46,
    SCANCODE_LEFTBRACKET = 47,
    SCANCODE_RIGHTBRACKET = 48,
    SCANCODE_BACKSLASH = 49, /**< Located at the lower left of the return
                                  *   key on ISO keyboards and at the right end
                                  *   of the QWERTY row on ANSI keyboards.
                                  *   Produces REVERSE SOLIDUS (backslash) and
                                  *   VERTICAL LINE in a US layout, REVERSE
                                  *   SOLIDUS and VERTICAL LINE in a UK Mac
                                  *   layout, NUMBER SIGN and TILDE in a UK
                                  *   Windows layout, DOLLAR SIGN and POUND SIGN
                                  *   in a Swiss German layout, NUMBER SIGN and
                                  *   APOSTROPHE in a German layout, GRAVE
                                  *   ACCENT and POUND SIGN in a French Mac
                                  *   layout, and ASTERISK and MICRO SIGN in a
                                  *   French Windows layout.
                                  */
    SCANCODE_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementer, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate SCANCODE_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    SCANCODE_SEMICOLON = 51,
    SCANCODE_APOSTROPHE = 52,
    SCANCODE_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                              *   and ISO keyboards). Produces GRAVE ACCENT and
                              *   TILDE in a US Windows layout and in US and UK
                              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                              *   and NOT SIGN in a UK Windows layout, SECTION
                              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                              *   layouts on ISO keyboards, SECTION SIGN and
                              *   DEGREE SIGN in a Swiss German layout (Mac:
                              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                              *   DEGREE SIGN in a German layout (Mac: only on
                              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                              *   French Windows layout, COMMERCIAL AT and
                              *   NUMBER SIGN in a French Mac layout on ISO
                              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                              *   SIGN in a Swiss German, German, or French Mac
                              *   layout on ANSI keyboards.
                              */
    SCANCODE_COMMA = 54,
    SCANCODE_PERIOD = 55,
    SCANCODE_SLASH = 56,

    SCANCODE_CAPSLOCK = 57,

    SCANCODE_F1 = 58,
    SCANCODE_F2 = 59,
    SCANCODE_F3 = 60,
    SCANCODE_F4 = 61,
    SCANCODE_F5 = 62,
    SCANCODE_F6 = 63,
    SCANCODE_F7 = 64,
    SCANCODE_F8 = 65,
    SCANCODE_F9 = 66,
    SCANCODE_F10 = 67,
    SCANCODE_F11 = 68,
    SCANCODE_F12 = 69,

    SCANCODE_PRINTSCREEN = 70,
    SCANCODE_SCROLLLOCK = 71,
    SCANCODE_PAUSE = 72,
    SCANCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    SCANCODE_HOME = 74,
    SCANCODE_PAGEUP = 75,
    SCANCODE_DELETE = 76,
    SCANCODE_END = 77,
    SCANCODE_PAGEDOWN = 78,
    SCANCODE_RIGHT = 79,
    SCANCODE_LEFT = 80,
    SCANCODE_DOWN = 81,
    SCANCODE_UP = 82,

    SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    SCANCODE_KP_DIVIDE = 84,
    SCANCODE_KP_MULTIPLY = 85,
    SCANCODE_KP_MINUS = 86,
    SCANCODE_KP_PLUS = 87,
    SCANCODE_KP_ENTER = 88,
    SCANCODE_KP_1 = 89,
    SCANCODE_KP_2 = 90,
    SCANCODE_KP_3 = 91,
    SCANCODE_KP_4 = 92,
    SCANCODE_KP_5 = 93,
    SCANCODE_KP_6 = 94,
    SCANCODE_KP_7 = 95,
    SCANCODE_KP_8 = 96,
    SCANCODE_KP_9 = 97,
    SCANCODE_KP_0 = 98,
    SCANCODE_KP_PERIOD = 99,

    SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Z.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    SCANCODE_APPLICATION = 101, /**< windows contextual menu, compose */
    SCANCODE_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    SCANCODE_KP_EQUALS = 103,
    SCANCODE_F13 = 104,
    SCANCODE_F14 = 105,
    SCANCODE_F15 = 106,
    SCANCODE_F16 = 107,
    SCANCODE_F17 = 108,
    SCANCODE_F18 = 109,
    SCANCODE_F19 = 110,
    SCANCODE_F20 = 111,
    SCANCODE_F21 = 112,
    SCANCODE_F22 = 113,
    SCANCODE_F23 = 114,
    SCANCODE_F24 = 115,
    SCANCODE_EXECUTE = 116,
    SCANCODE_HELP = 117,    /**< AL Integrated Help Center */
    SCANCODE_MENU = 118,    /**< Menu (show menu) */
    SCANCODE_SELECT = 119,
    SCANCODE_STOP = 120,    /**< AC Stop */
    SCANCODE_AGAIN = 121,   /**< AC Redo/Repeat */
    SCANCODE_UNDO = 122,    /**< AC Undo */
    SCANCODE_CUT = 123,     /**< AC Cut */
    SCANCODE_COPY = 124,    /**< AC Copy */
    SCANCODE_PASTE = 125,   /**< AC Paste */
    SCANCODE_FIND = 126,    /**< AC Find */
    SCANCODE_MUTE = 127,
    SCANCODE_VOLUMEUP = 128,
    SCANCODE_VOLUMEDOWN = 129,
    /* not sure whether there's a reason to enable these */
    /*     SCANCODE_LOCKINGCAPSLOCK = 130,  */
    /*     SCANCODE_LOCKINGNUMLOCK = 131, */
    /*     SCANCODE_LOCKINGSCROLLLOCK = 132, */
    SCANCODE_KP_COMMA = 133,
    SCANCODE_KP_EQUALSAS400 = 134,

    SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    SCANCODE_INTERNATIONAL2 = 136,
    SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
    SCANCODE_INTERNATIONAL4 = 138,
    SCANCODE_INTERNATIONAL5 = 139,
    SCANCODE_INTERNATIONAL6 = 140,
    SCANCODE_INTERNATIONAL7 = 141,
    SCANCODE_INTERNATIONAL8 = 142,
    SCANCODE_INTERNATIONAL9 = 143,
    SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
    SCANCODE_LANG2 = 145, /**< Hanja conversion */
    SCANCODE_LANG3 = 146, /**< Katakana */
    SCANCODE_LANG4 = 147, /**< Hiragana */
    SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
    SCANCODE_LANG6 = 149, /**< reserved */
    SCANCODE_LANG7 = 150, /**< reserved */
    SCANCODE_LANG8 = 151, /**< reserved */
    SCANCODE_LANG9 = 152, /**< reserved */

    SCANCODE_ALTERASE = 153,    /**< Erase-Eaze */
    SCANCODE_SYSREQ = 154,
    SCANCODE_CANCEL = 155,      /**< AC Cancel */
    SCANCODE_CLEAR = 156,
    SCANCODE_PRIOR = 157,
    SCANCODE_RETURN2 = 158,
    SCANCODE_SEPARATOR = 159,
    SCANCODE_OUT = 160,
    SCANCODE_OPER = 161,
    SCANCODE_CLEARAGAIN = 162,
    SCANCODE_CRSEL = 163,
    SCANCODE_EXSEL = 164,

    SCANCODE_KP_00 = 176,
    SCANCODE_KP_000 = 177,
    SCANCODE_THOUSANDSSEPARATOR = 178,
    SCANCODE_DECIMALSEPARATOR = 179,
    SCANCODE_CURRENCYUNIT = 180,
    SCANCODE_CURRENCYSUBUNIT = 181,
    SCANCODE_KP_LEFTPAREN = 182,
    SCANCODE_KP_RIGHTPAREN = 183,
    SCANCODE_KP_LEFTBRACE = 184,
    SCANCODE_KP_RIGHTBRACE = 185,
    SCANCODE_KP_TAB = 186,
    SCANCODE_KP_BACKSPACE = 187,
    SCANCODE_KP_A = 188,
    SCANCODE_KP_B = 189,
    SCANCODE_KP_C = 190,
    SCANCODE_KP_D = 191,
    SCANCODE_KP_E = 192,
    SCANCODE_KP_F = 193,
    SCANCODE_KP_XOR = 194,
    SCANCODE_KP_POWER = 195,
    SCANCODE_KP_PERCENT = 196,
    SCANCODE_KP_LESS = 197,
    SCANCODE_KP_GREATER = 198,
    SCANCODE_KP_AMPERSAND = 199,
    SCANCODE_KP_DBLAMPERSAND = 200,
    SCANCODE_KP_VERTICALBAR = 201,
    SCANCODE_KP_DBLVERTICALBAR = 202,
    SCANCODE_KP_COLON = 203,
    SCANCODE_KP_HASH = 204,
    SCANCODE_KP_SPACE = 205,
    SCANCODE_KP_AT = 206,
    SCANCODE_KP_EXCLAM = 207,
    SCANCODE_KP_MEMSTORE = 208,
    SCANCODE_KP_MEMRECALL = 209,
    SCANCODE_KP_MEMCLEAR = 210,
    SCANCODE_KP_MEMADD = 211,
    SCANCODE_KP_MEMSUBTRACT = 212,
    SCANCODE_KP_MEMMULTIPLY = 213,
    SCANCODE_KP_MEMDIVIDE = 214,
    SCANCODE_KP_PLUSMINUS = 215,
    SCANCODE_KP_CLEAR = 216,
    SCANCODE_KP_CLEARENTRY = 217,
    SCANCODE_KP_BINARY = 218,
    SCANCODE_KP_OCTAL = 219,
    SCANCODE_KP_DECIMAL = 220,
    SCANCODE_KP_HEXADECIMAL = 221,

    SCANCODE_LCTRL = 224,
    SCANCODE_LSHIFT = 225,
    SCANCODE_LALT = 226, /**< alt, option */
    SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
    SCANCODE_RCTRL = 228,
    SCANCODE_RSHIFT = 229,
    SCANCODE_RALT = 230, /**< alt gr, option */
    SCANCODE_RGUI = 231, /**< windows, command (apple), meta */

    SCANCODE_MODE = 257,    /**< I'm not sure if this is really not covered
                                 *   by any of the above, but since there's a
                                 *   special KMOD_MODE for it I'm adding it here
                                 */

                                 /* @} *//* Usage page 0x07 */

                                 /**
                                  *  \name Usage page 0x0C
                                  *
                                  *  These values are mapped from usage page 0x0C (USB consumer page).
                                  *
                                  *  There are way more keys in the spec than we can represent in the
                                  *  current scancode range, so pick the ones that commonly come up in
                                  *  real world usage.
                                  */
                                  /* @{ */

    SCANCODE_SLEEP = 258,                   /**< Sleep */
    SCANCODE_WAKE = 259,                    /**< Wake */

    SCANCODE_CHANNEL_INCREMENT = 260,       /**< Channel Increment */
    SCANCODE_CHANNEL_DECREMENT = 261,       /**< Channel Decrement */

    SCANCODE_MEDIA_PLAY = 262,          /**< Play */
    SCANCODE_MEDIA_PAUSE = 263,         /**< Pause */
    SCANCODE_MEDIA_RECORD = 264,        /**< Record */
    SCANCODE_MEDIA_FAST_FORWARD = 265,  /**< Fast Forward */
    SCANCODE_MEDIA_REWIND = 266,        /**< Rewind */
    SCANCODE_MEDIA_NEXT_TRACK = 267,    /**< Next Track */
    SCANCODE_MEDIA_PREVIOUS_TRACK = 268, /**< Previous Track */
    SCANCODE_MEDIA_STOP = 269,          /**< Stop */
    SCANCODE_MEDIA_EJECT = 270,         /**< Eject */
    SCANCODE_MEDIA_PLAY_PAUSE = 271,    /**< Play / Pause */
    SCANCODE_MEDIA_SELECT = 272,        /* Media Select */

    SCANCODE_AC_NEW = 273,              /**< AC New */
    SCANCODE_AC_OPEN = 274,             /**< AC Open */
    SCANCODE_AC_CLOSE = 275,            /**< AC Close */
    SCANCODE_AC_EXIT = 276,             /**< AC Exit */
    SCANCODE_AC_SAVE = 277,             /**< AC Save */
    SCANCODE_AC_PRINT = 278,            /**< AC Print */
    SCANCODE_AC_PROPERTIES = 279,       /**< AC Properties */

    SCANCODE_AC_SEARCH = 280,           /**< AC Search */
    SCANCODE_AC_HOME = 281,             /**< AC Home */
    SCANCODE_AC_BACK = 282,             /**< AC Back */
    SCANCODE_AC_FORWARD = 283,          /**< AC Forward */
    SCANCODE_AC_STOP = 284,             /**< AC Stop */
    SCANCODE_AC_REFRESH = 285,          /**< AC Refresh */
    SCANCODE_AC_BOOKMARKS = 286,        /**< AC Bookmarks */

    /* @} *//* Usage page 0x0C */


    /**
     *  \name Mobile keys
     *
     *  These are values that are often used on mobile phones.
     */
     /* @{ */

    SCANCODE_SOFTLEFT = 287, /**< Usually situated below the display on phones and
                                      used as a multi-function feature key for selecting
                                      a software defined function shown on the bottom left
                                      of the display. */
    SCANCODE_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                       used as a multi-function feature key for selecting
                                       a software defined function shown on the bottom right
                                       of the display. */
    SCANCODE_CALL = 289, /**< Used for accepting phone calls. */
    SCANCODE_ENDCALL = 290, /**< Used for rejecting phone calls. */

    /* @} *//* Mobile keys */

    /* Add any other keys here. */

    SCANCODE_RESERVED = 400,    /**< 400-500 reserved for dynamic keycodes */

    SCANCODE_COUNT = 512 /**< not a key, just marks the number of scancodes for array bounds */

} Scancode;

static const Scancode windows_scancode_table[] = {
    /*0x00*/ SCANCODE_UNKNOWN,
    /*0x01*/ SCANCODE_ESCAPE,
    /*0x02*/ SCANCODE_1,
    /*0x03*/ SCANCODE_2,
    /*0x04*/ SCANCODE_3,
    /*0x05*/ SCANCODE_4,
    /*0x06*/ SCANCODE_5,
    /*0x07*/ SCANCODE_6,
    /*0x08*/ SCANCODE_7,
    /*0x09*/ SCANCODE_8,
    /*0x0a*/ SCANCODE_9,
    /*0x0b*/ SCANCODE_0,
    /*0x0c*/ SCANCODE_MINUS,
    /*0x0d*/ SCANCODE_EQUALS,
    /*0x0e*/ SCANCODE_BACKSPACE,
    /*0x0f*/ SCANCODE_TAB,
    /*0x10*/ SCANCODE_Q,
    /*0x11*/ SCANCODE_W,
    /*0x12*/ SCANCODE_E,
    /*0x13*/ SCANCODE_R,
    /*0x14*/ SCANCODE_T,
    /*0x15*/ SCANCODE_Y,
    /*0x16*/ SCANCODE_U,
    /*0x17*/ SCANCODE_I,
    /*0x18*/ SCANCODE_O,
    /*0x19*/ SCANCODE_P,
    /*0x1a*/ SCANCODE_LEFTBRACKET,
    /*0x1b*/ SCANCODE_RIGHTBRACKET,
    /*0x1c*/ SCANCODE_RETURN,
    /*0x1d*/ SCANCODE_LCTRL,
    /*0x1e*/ SCANCODE_A,
    /*0x1f*/ SCANCODE_S,
    /*0x20*/ SCANCODE_D,
    /*0x21*/ SCANCODE_F,
    /*0x22*/ SCANCODE_G,
    /*0x23*/ SCANCODE_H,
    /*0x24*/ SCANCODE_J,
    /*0x25*/ SCANCODE_K,
    /*0x26*/ SCANCODE_L,
    /*0x27*/ SCANCODE_SEMICOLON,
    /*0x28*/ SCANCODE_APOSTROPHE,
    /*0x29*/ SCANCODE_GRAVE,
    /*0x2a*/ SCANCODE_LSHIFT,
    /*0x2b*/ SCANCODE_BACKSLASH,
    /*0x2c*/ SCANCODE_Z,
    /*0x2d*/ SCANCODE_X,
    /*0x2e*/ SCANCODE_C,
    /*0x2f*/ SCANCODE_V,
    /*0x30*/ SCANCODE_B,
    /*0x31*/ SCANCODE_N,
    /*0x32*/ SCANCODE_M,
    /*0x33*/ SCANCODE_COMMA,
    /*0x34*/ SCANCODE_PERIOD,
    /*0x35*/ SCANCODE_SLASH,
    /*0x36*/ SCANCODE_RSHIFT,
    /*0x37*/ SCANCODE_KP_MULTIPLY,
    /*0x38*/ SCANCODE_LALT,
    /*0x39*/ SCANCODE_SPACE,
    /*0x3a*/ SCANCODE_CAPSLOCK,
    /*0x3b*/ SCANCODE_F1,
    /*0x3c*/ SCANCODE_F2,
    /*0x3d*/ SCANCODE_F3,
    /*0x3e*/ SCANCODE_F4,
    /*0x3f*/ SCANCODE_F5,
    /*0x40*/ SCANCODE_F6,
    /*0x41*/ SCANCODE_F7,
    /*0x42*/ SCANCODE_F8,
    /*0x43*/ SCANCODE_F9,
    /*0x44*/ SCANCODE_F10,
    /*0x45*/ SCANCODE_NUMLOCKCLEAR,
    /*0x46*/ SCANCODE_SCROLLLOCK,
    /*0x47*/ SCANCODE_KP_7,
    /*0x48*/ SCANCODE_KP_8,
    /*0x49*/ SCANCODE_KP_9,
    /*0x4a*/ SCANCODE_KP_MINUS,
    /*0x4b*/ SCANCODE_KP_4,
    /*0x4c*/ SCANCODE_KP_5,
    /*0x4d*/ SCANCODE_KP_6,
    /*0x4e*/ SCANCODE_KP_PLUS,
    /*0x4f*/ SCANCODE_KP_1,
    /*0x50*/ SCANCODE_KP_2,
    /*0x51*/ SCANCODE_KP_3,
    /*0x52*/ SCANCODE_KP_0,
    /*0x53*/ SCANCODE_KP_PERIOD,
    /*0x54*/ SCANCODE_UNKNOWN,
    /*0x55*/ SCANCODE_UNKNOWN,
    /*0x56*/ SCANCODE_NONUSBACKSLASH,
    /*0x57*/ SCANCODE_F11,
    /*0x58*/ SCANCODE_F12,
    /*0x59*/ SCANCODE_KP_EQUALS,
    /*0x5a*/ SCANCODE_UNKNOWN,
    /*0x5b*/ SCANCODE_UNKNOWN,
    /*0x5c*/ SCANCODE_INTERNATIONAL6,
    /*0x5d*/ SCANCODE_UNKNOWN,
    /*0x5e*/ SCANCODE_UNKNOWN,
    /*0x5f*/ SCANCODE_UNKNOWN,
    /*0x60*/ SCANCODE_UNKNOWN,
    /*0x61*/ SCANCODE_UNKNOWN,
    /*0x62*/ SCANCODE_UNKNOWN,
    /*0x63*/ SCANCODE_UNKNOWN,
    /*0x64*/ SCANCODE_F13,
    /*0x65*/ SCANCODE_F14,
    /*0x66*/ SCANCODE_F15,
    /*0x67*/ SCANCODE_F16,
    /*0x68*/ SCANCODE_F17,
    /*0x69*/ SCANCODE_F18,
    /*0x6a*/ SCANCODE_F19,
    /*0x6b*/ SCANCODE_F20,
    /*0x6c*/ SCANCODE_F21,
    /*0x6d*/ SCANCODE_F22,
    /*0x6e*/ SCANCODE_F23,
    /*0x6f*/ SCANCODE_UNKNOWN,
    /*0x70*/ SCANCODE_INTERNATIONAL2,
    /*0x71*/ SCANCODE_LANG2,
    /*0x72*/ SCANCODE_LANG1,
    /*0x73*/ SCANCODE_INTERNATIONAL1,
    /*0x74*/ SCANCODE_UNKNOWN,
    /*0x75*/ SCANCODE_UNKNOWN,
    /*0x76*/ SCANCODE_F24,
    /*0x77*/ SCANCODE_LANG4,
    /*0x78*/ SCANCODE_LANG3,
    /*0x79*/ SCANCODE_INTERNATIONAL4,
    /*0x7a*/ SCANCODE_UNKNOWN,
    /*0x7b*/ SCANCODE_INTERNATIONAL5,
    /*0x7c*/ SCANCODE_UNKNOWN,
    /*0x7d*/ SCANCODE_INTERNATIONAL3,
    /*0x7e*/ SCANCODE_KP_COMMA,
    /*0x7f*/ SCANCODE_UNKNOWN,
    /*0xe000*/ SCANCODE_UNKNOWN,
    /*0xe001*/ SCANCODE_UNKNOWN,
    /*0xe002*/ SCANCODE_UNKNOWN,
    /*0xe003*/ SCANCODE_UNKNOWN,
    /*0xe004*/ SCANCODE_UNKNOWN,
    /*0xe005*/ SCANCODE_UNKNOWN,
    /*0xe006*/ SCANCODE_UNKNOWN,
    /*0xe007*/ SCANCODE_UNKNOWN,
    /*0xe008*/ SCANCODE_UNKNOWN,
    /*0xe009*/ SCANCODE_UNKNOWN,
    /*0xe00a*/ SCANCODE_PASTE,
    /*0xe00b*/ SCANCODE_UNKNOWN,
    /*0xe00c*/ SCANCODE_UNKNOWN,
    /*0xe00d*/ SCANCODE_UNKNOWN,
    /*0xe00e*/ SCANCODE_UNKNOWN,
    /*0xe00f*/ SCANCODE_UNKNOWN,
    /*0xe010*/ SCANCODE_MEDIA_PREVIOUS_TRACK,
    /*0xe011*/ SCANCODE_UNKNOWN,
    /*0xe012*/ SCANCODE_UNKNOWN,
    /*0xe013*/ SCANCODE_UNKNOWN,
    /*0xe014*/ SCANCODE_UNKNOWN,
    /*0xe015*/ SCANCODE_UNKNOWN,
    /*0xe016*/ SCANCODE_UNKNOWN,
    /*0xe017*/ SCANCODE_CUT,
    /*0xe018*/ SCANCODE_COPY,
    /*0xe019*/ SCANCODE_MEDIA_NEXT_TRACK,
    /*0xe01a*/ SCANCODE_UNKNOWN,
    /*0xe01b*/ SCANCODE_UNKNOWN,
    /*0xe01c*/ SCANCODE_KP_ENTER,
    /*0xe01d*/ SCANCODE_RCTRL,
    /*0xe01e*/ SCANCODE_UNKNOWN,
    /*0xe01f*/ SCANCODE_UNKNOWN,
    /*0xe020*/ SCANCODE_MUTE,
    /*0xe021*/ SCANCODE_UNKNOWN, // LaunchApp2
    /*0xe022*/ SCANCODE_MEDIA_PLAY_PAUSE,
    /*0xe023*/ SCANCODE_UNKNOWN,
    /*0xe024*/ SCANCODE_MEDIA_STOP,
    /*0xe025*/ SCANCODE_UNKNOWN,
    /*0xe026*/ SCANCODE_UNKNOWN,
    /*0xe027*/ SCANCODE_UNKNOWN,
    /*0xe028*/ SCANCODE_UNKNOWN,
    /*0xe029*/ SCANCODE_UNKNOWN,
    /*0xe02a*/ SCANCODE_UNKNOWN,
    /*0xe02b*/ SCANCODE_UNKNOWN,
    /*0xe02c*/ SCANCODE_MEDIA_EJECT,
    /*0xe02d*/ SCANCODE_UNKNOWN,
    /*0xe02e*/ SCANCODE_VOLUMEDOWN,
    /*0xe02f*/ SCANCODE_UNKNOWN,
    /*0xe030*/ SCANCODE_VOLUMEUP,
    /*0xe031*/ SCANCODE_UNKNOWN,
    /*0xe032*/ SCANCODE_AC_HOME,
    /*0xe033*/ SCANCODE_UNKNOWN,
    /*0xe034*/ SCANCODE_UNKNOWN,
    /*0xe035*/ SCANCODE_KP_DIVIDE,
    /*0xe036*/ SCANCODE_UNKNOWN,
    /*0xe037*/ SCANCODE_PRINTSCREEN,
    /*0xe038*/ SCANCODE_RALT,
    /*0xe039*/ SCANCODE_UNKNOWN,
    /*0xe03a*/ SCANCODE_UNKNOWN,
    /*0xe03b*/ SCANCODE_HELP,
    /*0xe03c*/ SCANCODE_UNKNOWN,
    /*0xe03d*/ SCANCODE_UNKNOWN,
    /*0xe03e*/ SCANCODE_UNKNOWN,
    /*0xe03f*/ SCANCODE_UNKNOWN,
    /*0xe040*/ SCANCODE_UNKNOWN,
    /*0xe041*/ SCANCODE_UNKNOWN,
    /*0xe042*/ SCANCODE_UNKNOWN,
    /*0xe043*/ SCANCODE_UNKNOWN,
    /*0xe044*/ SCANCODE_UNKNOWN,
    /*0xe045*/ SCANCODE_NUMLOCKCLEAR,
    /*0xe046*/ SCANCODE_PAUSE,
    /*0xe047*/ SCANCODE_HOME,
    /*0xe048*/ SCANCODE_UP,
    /*0xe049*/ SCANCODE_PAGEUP,
    /*0xe04a*/ SCANCODE_UNKNOWN,
    /*0xe04b*/ SCANCODE_LEFT,
    /*0xe04c*/ SCANCODE_UNKNOWN,
    /*0xe04d*/ SCANCODE_RIGHT,
    /*0xe04e*/ SCANCODE_UNKNOWN,
    /*0xe04f*/ SCANCODE_END,
    /*0xe050*/ SCANCODE_DOWN,
    /*0xe051*/ SCANCODE_PAGEDOWN,
    /*0xe052*/ SCANCODE_INSERT,
    /*0xe053*/ SCANCODE_DELETE,
    /*0xe054*/ SCANCODE_UNKNOWN,
    /*0xe055*/ SCANCODE_UNKNOWN,
    /*0xe056*/ SCANCODE_UNKNOWN,
    /*0xe057*/ SCANCODE_UNKNOWN,
    /*0xe058*/ SCANCODE_UNKNOWN,
    /*0xe059*/ SCANCODE_UNKNOWN,
    /*0xe05a*/ SCANCODE_UNKNOWN,
    /*0xe05b*/ SCANCODE_LGUI,
    /*0xe05c*/ SCANCODE_RGUI,
    /*0xe05d*/ SCANCODE_APPLICATION,
    /*0xe05e*/ SCANCODE_POWER,
    /*0xe05f*/ SCANCODE_SLEEP,
    /*0xe060*/ SCANCODE_UNKNOWN,
    /*0xe061*/ SCANCODE_UNKNOWN,
    /*0xe062*/ SCANCODE_UNKNOWN,
    /*0xe063*/ SCANCODE_UNKNOWN,
    /*0xe064*/ SCANCODE_UNKNOWN,
    /*0xe065*/ SCANCODE_AC_SEARCH,
    /*0xe066*/ SCANCODE_AC_BOOKMARKS,
    /*0xe067*/ SCANCODE_AC_REFRESH,
    /*0xe068*/ SCANCODE_AC_STOP,
    /*0xe069*/ SCANCODE_AC_FORWARD,
    /*0xe06a*/ SCANCODE_AC_BACK,
    /*0xe06b*/ SCANCODE_UNKNOWN,    // LaunchApp1
    /*0xe06c*/ SCANCODE_UNKNOWN,    // LaunchMail
    /*0xe06d*/ SCANCODE_MEDIA_SELECT,
    /*0xe06e*/ SCANCODE_UNKNOWN,
    /*0xe06f*/ SCANCODE_UNKNOWN,
    /*0xe070*/ SCANCODE_UNKNOWN,
    /*0xe071*/ SCANCODE_UNKNOWN,
    /*0xe072*/ SCANCODE_UNKNOWN,
    /*0xe073*/ SCANCODE_UNKNOWN,
    /*0xe074*/ SCANCODE_UNKNOWN,
    /*0xe075*/ SCANCODE_UNKNOWN,
    /*0xe076*/ SCANCODE_UNKNOWN,
    /*0xe077*/ SCANCODE_UNKNOWN,
    /*0xe078*/ SCANCODE_UNKNOWN,
    /*0xe079*/ SCANCODE_UNKNOWN,
    /*0xe07a*/ SCANCODE_UNKNOWN,
    /*0xe07b*/ SCANCODE_UNKNOWN,
    /*0xe07c*/ SCANCODE_UNKNOWN,
    /*0xe07d*/ SCANCODE_UNKNOWN,
    /*0xe07e*/ SCANCODE_UNKNOWN,
    /*0xe07f*/ SCANCODE_UNKNOWN
};