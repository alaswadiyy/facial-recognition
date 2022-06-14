"Arduino.h"
#include "fd_forward.h"
#include "fr_forward.h"
#include "fr_flash.h"
#define ENROLL_CONFIRM_TIMES 5
#define FACE_ID_SAVE_NUMBER 7
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"
using namespace websockets;
WebsocketsServer socket_server;

camera_fb_t * fb = NULL;

uint8_t connection_state = 0;
uint16_t reconnect_interval = 10000;
EMailSender emailSend("lasuepeproject@gmail.com", "upqxkmuayjxnijjo");
//#include "BluetoothSerial.h"
//BluetoothSerial SerialBT;








long current_millis;
long last_detected_millis = 0;
void app_facenet_main();
void app_httpserver_init();

typedef struct
{
  uint8_t *image;
  box_array_t *net_boxes;
  dl_matrix3d_t *face_id;
} http_img_process_result;


static inline mtmn_config_t app_mtmn_config()
{
  mtmn_config_t mtmn_config = {0};
  mtmn_config.type = FAST;
  mtmn_config.min_face = 80;
  mtmn_config.pyramid = 0.707;
  mtmn_config.pyramid_times = 4;
  mtmn_config.p_threshold.score = 0.6;
  mtmn_config.p_threshold.nms = 0.7;
  mtmn_config.p_threshold.candidate_number = 20;
  mtmn_config.r_threshold.score = 0.7;
  mtmn_config.r_threshold.nms = 0.7;
  mtmn_config.r_threshold.candidate_number = 10;
  mtmn_config.o_threshold.score = 0.7;
  mtmn_config.o_threshold.nms = 0.7;
  mtmn_config.o_threshold.candidate_number = 1;
  return mtmn_config;
}
mtmn_config_t mtmn_config = app_mtmn_config();

face_id_name_list st_face_list;
static dl_matrix3du_t *aligned_face = NULL;

httpd_handle_t camera_httpd = NULL;

typedef enum
{
  START_STREAM,
  START_DETECT,
  SHOW_FACES,
  START_RECOGNITION,
  START_ENROLL,
  ENROLL_COMPLETE,
  DELETE_ALL,
} en_fsm_state;
en_fsm_state g_state;

typedef struct
{
  char enroll_name[ENROLL_NAME_LEN];
} httpd_resp_value;

httpd_resp_value st_name;



//#include <PS2Kbd.h>
//#define KEYBOARD_DATA 13
//#define KEYBOARD_CLK  2
//PS2Kbd keyboard(KEYBOARD_DATA, KEYBOARD_CLK);
#include <EEPROM.h>
#define SerialBT Serial
char lcd_putc[20];
char btt[50];
 char rfid_buf[20];
#define clk 2   //green                                           
#define daat 13  //white
#ifndef PS2Keyboard_h
#define PS2Keyboard_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h" // for attachInterrupt, FALLING
#else
#include "WProgram.h"
#endif
uint8_t k,ka;
// interrupt pins for known boards

// Teensy and maybe others automatically define this info
#if !defined(CORE_INT0_PIN) && !defined(CORE_INT1_PIN) && !defined(CORE_INT2_PIN)&& !defined(CORE_INT3_PIN)

// Arduino Mega
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega
  #define CORE_INT0_PIN  2
  #define CORE_INT1_PIN  3
  #define CORE_INT2_PIN  21
  #define CORE_INT3_PIN  20
  #define CORE_INT4_PIN  19
  #define CORE_INT5_PIN  18

// Arduino Due (untested)
#elif defined(__SAM3X8E__) 
  #define CORE_INT_EVERY_PIN
  #ifndef PROGMEM
  #define PROGMEM
  #endif
  #ifndef pgm_read_byte
  #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
  #endif

// Arduino Leonardo (untested)
#elif defined(__AVR_ATmega32U4__) && !defined(CORE_TEENSY)
  #define CORE_INT0_PIN  3
  #define CORE_INT1_PIN  2
  #define CORE_INT2_PIN  0
  #define CORE_INT3_PIN  1

// Sanguino (untested)
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__) // Sanguino
  #define CORE_INT0_PIN  10
  #define CORE_INT1_PIN  11
  #define CORE_INT2_PIN  2

// Chipkit Uno32 (untested)
#elif defined(__PIC32MX__) && defined(_BOARD_UNO_)
  #define CORE_INT0_PIN  38
  #define CORE_INT1_PIN  2
  #define CORE_INT2_PIN  7
  #define CORE_INT3_PIN  8
  #define CORE_INT4_PIN  35

// Chipkit Mega32 (untested)
#elif defined(__PIC32MX__) && defined(_BOARD_MEGA_)
  #define CORE_INT0_PIN  3
  #define CORE_INT1_PIN  2
  #define CORE_INT2_PIN  7
  #define CORE_INT3_PIN  21
  #define CORE_INT4_PIN  20

// http://hlt.media.mit.edu/?p=1229
#elif defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  #define CORE_INT0_PIN  2

// Arduino Uno, Duemilanove, LilyPad, Mini, Fio, etc...
#else
  #define CORE_INT0_PIN  2
  #define CORE_INT1_PIN  3

#endif
#endif


// Every call to read() returns a single byte for each
// keystroke.  These configure what byte will be returned
// for each "special" key.  To ignore a key, use zero.
#define PS2_TAB            9
#define PS2_ENTER         13
#define PS2_BACKSPACE      8 //  127
#define PS2_ESC           205// 27
#define PS2_INSERT         0
#define PS2_DELETE         127
#define PS2_HOME         0
#define PS2_END            0
#define PS2_PAGEUP         25
#define PS2_PAGEDOWN         26
#define PS2_UPARROW         192
#define PS2_LEFTARROW         190
#define PS2_DOWNARROW         190
#define PS2_RIGHTARROW         192
#define PS2_F1            0
#define PS2_F2            0
#define PS2_F3            0
#define PS2_F4            0
#define PS2_F5            0
#define PS2_F6            0
#define PS2_F7            0
#define PS2_F8            0
#define PS2_F9            0
#define PS2_F10            0
#define PS2_F11            0
#define PS2_F12            0
#define PS2_SCROLL         0

#define PS2_INVERTED_EXCLAMATION   161 // Â¡
#define PS2_CENT_SIGN         162 // Â¢
#define PS2_POUND_SIGN         163 // Â£
#define PS2_CURRENCY_SIGN      164 // Â¤
#define PS2_YEN_SIGN         165 // Â¥
#define PS2_BROKEN_BAR                    166 // Â¦
#define PS2_SECTION_SIGN      167 // Â§
#define PS2_DIAERESIS         168 // Â¨
#define PS2_COPYRIGHT_SIGN      169 // Â©
#define PS2_FEMININE_ORDINAL      170 // Âª
#define PS2_LEFT_DOUBLE_ANGLE_QUOTE   171 // Â«
#define PS2_NOT_SIGN         172 // Â¬
#define PS2_HYPHEN         173
#define PS2_REGISTERED_SIGN      174 // Â®
#define PS2_MACRON         175 // Â¯
#define PS2_DEGREE_SIGN         176 // Â°
#define PS2_PLUS_MINUS_SIGN      177 // Â±
#define PS2_SUPERSCRIPT_TWO      178 // Â²
#define PS2_SUPERSCRIPT_THREE      179 // Â³
#define PS2_ACUTE_ACCENT      180 // Â´
#define PS2_MICRO_SIGN         181 // Âµ
#define PS2_PILCROW_SIGN      182 // Â¶
#define PS2_MIDDLE_DOT         183 // Â·
#define PS2_CEDILLA         184 // Â¸
#define PS2_SUPERSCRIPT_ONE      185 // Â¹
#define PS2_MASCULINE_ORDINAL      186 // Âº
#define PS2_RIGHT_DOUBLE_ANGLE_QUOTE   187 // Â»
#define PS2_FRACTION_ONE_QUARTER   188 // Â¼
#define PS2_FRACTION_ONE_HALF      189 // Â½
#define PS2_FRACTION_THREE_QUARTERS   190 // Â¾
#define PS2_INVERTED_QUESTION MARK   191 // Â¿
#define PS2_A_GRAVE         192 // Ã€
#define PS2_A_ACUTE         193 // Ã
#define PS2_A_CIRCUMFLEX      194 // Ã‚
#define PS2_A_TILDE         195 // Ãƒ
#define PS2_A_DIAERESIS         196 // Ã„
#define PS2_A_RING_ABOVE      197 // Ã…
#define PS2_AE            198 // Ã†
#define PS2_C_CEDILLA         199 // Ã‡
#define PS2_E_GRAVE         200 // Ãˆ
#define PS2_E_ACUTE         201 // Ã‰
#define PS2_E_CIRCUMFLEX      202 // ÃŠ
#define PS2_E_DIAERESIS         203 // Ã‹
#define PS2_I_GRAVE         204 // ÃŒ
#define PS2_I_ACUTE         205 // Ã
#define PS2_I_CIRCUMFLEX      206 // ÃŽ
#define PS2_I_DIAERESIS         207 // Ã
#define PS2_ETH            208 // Ã
#define PS2_N_TILDE         209 // Ã‘
#define PS2_O_GRAVE         210 // Ã’
#define PS2_O_ACUTE         211 // Ã“
#define PS2_O_CIRCUMFLEX      212 // Ã”
#define PS2_O_TILDE         213 // Ã•
#define PS2_O_DIAERESIS         214 // Ã–
#define PS2_MULTIPLICATION      215 // Ã—
#define PS2_O_STROKE         216 // Ã˜
#define PS2_U_GRAVE         217 // Ã™
#define PS2_U_ACUTE         218 // Ãš
#define PS2_U_CIRCUMFLEX      219 // Ã›
#define PS2_U_DIAERESIS         220 // Ãœ
#define PS2_Y_ACUTE         221 // Ã
#define PS2_THORN         222 // Ãž
#define PS2_SHARP_S         223 // ÃŸ
#define PS2_a_GRAVE         224 // Ã 
#define PS2_a_ACUTE         225 // Ã¡
#define PS2_a_CIRCUMFLEX      226 // Ã¢
#define PS2_a_TILDE         227 // Ã£
#define PS2_a_DIAERESIS         228 // Ã¤
#define PS2_a_RING_ABOVE      229 // Ã¥
#define PS2_ae            230 // Ã¦
#define PS2_c_CEDILLA         231 // Ã§
#define PS2_e_GRAVE         232 // Ã¨
#define PS2_e_ACUTE         233 // Ã©
#define PS2_e_CIRCUMFLEX      234 // Ãª
#define PS2_e_DIAERESIS         235 // Ã«
#define PS2_i_GRAVE         236 // Ã¬
#define PS2_i_ACUTE         237 // Ã­
#define PS2_i_CIRCUMFLEX      238 // Ã®
#define PS2_i_DIAERESIS         239 // Ã¯
#define PS2_eth            240 // Ã°
#define PS2_n_TILDE         241 // Ã±
#define PS2_o_GRAVE         242 // Ã²
#define PS2_o_ACUTE         243 // Ã³
#define PS2_o_CIRCUMFLEX      244 // Ã´
#define PS2_o_TILDE         245 // Ãµ
#define PS2_o_DIAERESIS         246 // Ã¶
#define PS2_DIVISION         247 // Ã·
#define PS2_o_STROKE         248 // Ã¸
#define PS2_u_GRAVE         249 // Ã¹
#define PS2_u_ACUTE         250 // Ãº
#define PS2_u_CIRCUMFLEX      251 // Ã»
#define PS2_u_DIAERESIS         252 // Ã¼
#define PS2_y_ACUTE         253 // Ã½
#define PS2_thorn         254 // Ã¾
#define PS2_y_DIAERESIS         255 // Ã¿


#define PS2_KEYMAP_SIZE 136

typedef struct {
   uint8_t noshift[PS2_KEYMAP_SIZE];
   uint8_t shift[PS2_KEYMAP_SIZE];
   uint8_t uses_altgr;
   uint8_t altgr[PS2_KEYMAP_SIZE];
} PS2Keymap_t;


extern const PROGMEM PS2Keymap_t PS2Keymap_US;
extern const PROGMEM PS2Keymap_t PS2Keymap_German;
extern const PROGMEM PS2Keymap_t PS2Keymap_French;


/**
 * Purpose: Provides an easy access to PS2 keyboards
 * Author:  Christian Weichel
 */
class PS2Keyboard {
  public:
     /**
      * This constructor does basically nothing. Please call the begin(int,int)
      * method before using any other method of this class.
      */
    PS2Keyboard();
    
    /**
     * Starts the keyboard "service" by registering the external interrupt.
     * setting the pin modes correctly and driving those needed to high.
     * The propably best place to call this method is in the setiup routine.
     */
    static void begin( const PS2Keymap_t &map = PS2Keymap_US);
    
    /**
     * Returns true if there is a char to be read, false if not.
     */
    static bool available();
    
    /**
     * Returns the char last read from the keyboard.
     * If there is no char available, -1 is returned.
     */
    static int read();
    static int readUnicode();
};

#endif

#define BUFFER_SIZE 45
static volatile uint8_t buffer[BUFFER_SIZE];
static volatile uint8_t head, tail;

static uint8_t CharBuffer=0;
static uint8_t UTF8next=0;
static const PS2Keymap_t *keymap=NULL;

// The ISR for the external interrupt
//ICACHE_RAM_ATTR void ps2interrupt(void)
void IRAM_ATTR  ps2interrupt(void* arg)
{
  static uint8_t bitcount=0;
  static uint8_t incoming=0;
  static uint32_t prev_ms=0;
  uint32_t now_ms;
  uint8_t n, val;

  val = digitalRead(daat);
  now_ms = millis();
  if (now_ms - prev_ms > 250) {
    bitcount = 0;
    incoming = 0;
  }
  prev_ms = now_ms;
  n = bitcount - 1;
  if (n <= 7) {
    incoming |= (val << n);
  }
  bitcount++;
  if (bitcount == 11) {
    uint8_t i = head + 1;
    if (i >= BUFFER_SIZE) i = 0;
    if (i != tail) {
      buffer[i] = incoming;
      head = i;
    }
    bitcount = 0;
    incoming = 0;
  }
}

static inline uint8_t get_scan_code(void)
{
  uint8_t c, i;

  i = tail;
  if (i == head) return 0;
  i++;
  if (i >= BUFFER_SIZE) i = 0;
  c = buffer[i];
  tail = i;
  return c;
}

// http://www.quadibloc.com/comp/scan.htm
// http://www.computer-engineering.org/ps2keyboard/scancodes2.html

// These arrays provide a simple key map, to turn scan codes into ISO8859
// output.  If a non-US keyboard is used, these may need to be modified
// for the desired output.
//

const PROGMEM PS2Keymap_t PS2Keymap_US = {
  // without shift
  {0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
  0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '`', 0,
  0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'q', '1', 0,
  0, 0, 'z', 's', 'a', 'w', '2', 0,
  0, 'c', 'x', 'd', 'e', '4', '3', 0,
  0, ' ', 'v', 'f', 't', 'r', '5', 0,
  0, 'n', 'b', 'h', 'g', 'y', '6', 0,
  0, 0, 'm', 'j', 'u', '7', '8', 0,
  0, ',', 'k', 'i', 'o', '0', '9', 0,
  0, '.', '/', 'l', ';', 'p', '-', 0,
  0, 0, '\'', 0, '[', '=', 0, 0,
   6/*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, ']', 0, '\\', 0, 0,
  0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
  0, '1', 0, '4', '7', 0, 0, 0,
  '0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
  PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
  0, 0, 0, PS2_F7 },
  // with shift
  {0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
  0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '~', 0,
  0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'Q', '!', 0,
  0, 0, 'Z', 'S', 'A', 'W', '@', 0,
  0, 'C', 'X', 'D', 'E', '$', '#', 0,
  0, ' ', 'V', 'F', 'T', 'R', '%', 0,
  0, 'N', 'B', 'H', 'G', 'Y', '^', 0,
  0, 0, 'M', 'J', 'U', '&', '*', 0,
  0, '<', 'K', 'I', 'O', ')', '(', 0,
  0, '>', '?', 'L', ':', 'P', '_', 0,
  0, 0, '"', 0, '{', '+', 0, 0,
  6 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '}', 0, '|', 0, 0,
  0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
  0, '1', 0, '4', '7', 0, 0, 0,
  '0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
  PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
  0, 0, 0, PS2_F7 },
  0
};


const PROGMEM PS2Keymap_t PS2Keymap_German = {
  // without shift
  {0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
  0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '^', 0,
  0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'q', '1', 0,
  0, 0, 'y', 's', 'a', 'w', '2', 0,
  0, 'c', 'x', 'd', 'e', '4', '3', 0,
  0, ' ', 'v', 'f', 't', 'r', '5', 0,
  0, 'n', 'b', 'h', 'g', 'z', '6', 0,
  0, 0, 'm', 'j', 'u', '7', '8', 0,
  0, ',', 'k', 'i', 'o', '0', '9', 0,
  0, '.', '-', 'l', PS2_o_DIAERESIS, 'p', PS2_SHARP_S, 0,
  0, 0, PS2_a_DIAERESIS, 0, PS2_u_DIAERESIS, '\'', 0, 0,
  200 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '+', 0, '#', 0, 0,
  0, '<', 0, 0, 0, 0, PS2_BACKSPACE, 0,
  0, '1', 0, '4', '7', 0, 0, 0,
  '0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
  PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
  0, 0, 0, PS2_F7 },
  // with shift
  {0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
  0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, PS2_DEGREE_SIGN, 0,
  0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'Q', '!', 0,
  0, 0, 'Y', 'S', 'A', 'W', '"', 0,
  0, 'C', 'X', 'D', 'E', '$', PS2_SECTION_SIGN, 0,
  0, ' ', 'V', 'F', 'T', 'R', '%', 0,
  0, 'N', 'B', 'H', 'G', 'Z', '&', 0,
  0, 0, 'M', 'J', 'U', '/', '(', 0,
  0, ';', 'K', 'I', 'O', '=', ')', 0,
  0, ':', '_', 'L', PS2_O_DIAERESIS, 'P', '?', 0,
  0, 0, PS2_A_DIAERESIS, 0, PS2_U_DIAERESIS, '`', 0, 0,
  0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '*', 0, '\'', 0, 0,
  0, '>', 0, 0, 0, 0, PS2_BACKSPACE, 0,
  0, '1', 0, '4', '7', 0, 0, 0,
  '0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
  PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
  0, 0, 0, PS2_F7 },
  1,
  // with altgr
  {0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
  0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, 0, 0,
  0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, '@', 0, 0,
  0, 0, 0, 0, 0, 0, PS2_SUPERSCRIPT_TWO, 0,
  0, 0, 0, 0, PS2_CURRENCY_SIGN, 0, PS2_SUPERSCRIPT_THREE, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, PS2_MICRO_SIGN, 0, 0, '{', '[', 0,
  0, 0, 0, 0, 0, '}', ']', 0,
  0, 0, 0, 0, 0, 0, '\\', 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  200 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '~', 0, '#', 0, 0,
  0, '|', 0, 0, 0, 0, PS2_BACKSPACE, 0,
  0, '1', 0, '4', '7', 0, 0, 0,
  '0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
  PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
  0, 0, 0, PS2_F7 }
};


const PROGMEM PS2Keymap_t PS2Keymap_French = {
  // without shift
  {0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
  0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, PS2_SUPERSCRIPT_TWO, 0,
  0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'a', '&', 0,
  0, 0, 'w', 's', 'q', 'z', PS2_e_ACUTE, 0,
  0, 'c', 'x', 'd', 'e', '\'', '"', 0,
  0, ' ', 'v', 'f', 't', 'r', '(', 0,
  0, 'n', 'b', 'h', 'g', 'y', '-', 0,
  0, 0, ',', 'j', 'u', PS2_e_GRAVE, '_', 0,
  0, ';', 'k', 'i', 'o', PS2_a_GRAVE, PS2_c_CEDILLA, 0,
  0, ':', '!', 'l', 'm', 'p', ')', 0,
  0, 0, PS2_u_GRAVE, 0, '^', '=', 0, 0,
  0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '$', 0, '*', 0, 0,
  0, '<', 0, 0, 0, 0, PS2_BACKSPACE, 0,
  0, '1', 0, '4', '7', 0, 0, 0,
  '0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
  PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
  0, 0, 0, PS2_F7 },
  // with shift
  {0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
  0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, 0, 0,
  0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'A', '1', 0,
  0, 0, 'W', 'S', 'Q', 'Z', '2', 0,
  0, 'C', 'X', 'D', 'E', '4', '3', 0,
  0, ' ', 'V', 'F', 'T', 'R', '5', 0,
  0, 'N', 'B', 'H', 'G', 'Y', '6', 0,
  0, 0, '?', 'J', 'U', '7', '8', 0,
  0, '.', 'K', 'I', 'O', '0', '9', 0,
  0, '/', PS2_SECTION_SIGN, 'L', 'M', 'P', PS2_DEGREE_SIGN, 0,
  0, 0, '%', 0, PS2_DIAERESIS, '+', 0, 0,
  0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, PS2_POUND_SIGN, 0, PS2_MICRO_SIGN, 0, 0,
  0, '>', 0, 0, 0, 0, PS2_BACKSPACE, 0,
  0, '1', 0, '4', '7', 0, 0, 0,
  '0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
  PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
  0, 0, 0, PS2_F7 },
  1,
  // with altgr
  {0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
  0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, 0, 0,
  0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, '@', 0, 0,
  0, 0, 0, 0, 0, 0, '~', 0,
  0, 0, 0, 0, 0 /*PS2_EURO_SIGN*/, '{', '#', 0,
  0, 0, 0, 0, 0, 0, '[', 0,
  0, 0, 0, 0, 0, 0, '|', 0,
  0, 0, 0, 0, 0, '`', '\\', 0,
  0, 0, 0, 0, 0, '@', '^', 0,
  0, 0, 0, 0, 0, 0, ']', 0,
  0, 0, 0, 0, 0, 0, '}', 0,
  0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '¤', 0, '#', 0, 0,
  0, '|', 0, 0, 0, 0, PS2_BACKSPACE, 0,
  0, '1', 0, '4', '7', 0, 0, 0,
  '0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
  PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
  0, 0, 0, PS2_F7 }
};


#define BREAK     0x01
#define MODIFIER  0x02
#define SHIFT_L   0x04
#define SHIFT_R   0x08
#define ALTGR     0x10

static char get_iso8859_code(void)
{
  static uint8_t state=0;
  uint8_t s;
  char c;

  while (1) {
    s = get_scan_code();
    if (!s) return 0;
    if (s == 0xF0) {
      state |= BREAK;
    } else if (s == 0xE0) {
      state |= MODIFIER;
    } else {
      if (state & BREAK) {
        if (s == 0x12) {
          state &= ~SHIFT_L;
        } else if (s == 0x59) {
          state &= ~SHIFT_R;
        } else if (s == 0x11 && (state & MODIFIER)) {
          state &= ~ALTGR;
        }
        // CTRL, ALT & WIN keys could be added
        // but is that really worth the overhead?
        state &= ~(BREAK | MODIFIER);
        continue;
      }
      if (s == 0x12) {
        state |= SHIFT_L;
        continue;
      } else if (s == 0x59) {
        state |= SHIFT_R;
        continue;
      } else if (s == 0x11 && (state & MODIFIER)) {
        state |= ALTGR;
      }
      c = 0;
      if (state & MODIFIER) {
        switch (s) {
          case 0x70: c = PS2_INSERT;      break;
          case 0x6C: c = PS2_HOME;        break;
          case 0x7D: c = PS2_PAGEUP;      break;
          case 0x71: c = PS2_DELETE;      break;
          case 0x69: c = PS2_END;         break;
          case 0x7A: c = PS2_PAGEDOWN;    break;
          case 0x75: c = PS2_UPARROW;     break;
          case 0x6B: c = PS2_LEFTARROW;   break;
          case 0x72: c = PS2_DOWNARROW;   break;
          case 0x74: c = PS2_RIGHTARROW;  break;
          case 0x4A: c = '/';             break;
          case 0x5A: c = PS2_ENTER;       break;
          default: break;
        }
      } else if ((state & ALTGR) && keymap->uses_altgr) {
        if (s < PS2_KEYMAP_SIZE)
          c = pgm_read_byte(keymap->altgr + s);
      } else if (state & (SHIFT_L | SHIFT_R)) {
        if (s < PS2_KEYMAP_SIZE)
          c = pgm_read_byte(keymap->shift + s);
      } else {
        if (s < PS2_KEYMAP_SIZE)
          c = pgm_read_byte(keymap->noshift + s);
      }
      state &= ~(BREAK | MODIFIER);
      if (c) return c;
    }
  }
}

bool PS2Keyboard::available() {
  if (CharBuffer || UTF8next) return true;
  CharBuffer = get_iso8859_code();
  if (CharBuffer) return true;
  return false;
}

int PS2Keyboard::read() {
  uint8_t result;

  result = UTF8next;
  if (result) {
    UTF8next = 0;
  } else {
    result = CharBuffer;
    if (result) {
      CharBuffer = 0;
    } else {
      result = get_iso8859_code();
    }
    if (result >= 128) {
      UTF8next = (result & 0x3F) | 0x80;
      result = ((result >> 6) & 0x1F) | 0xC0;
    }
  }
  if (!result) return -1;
  return result;
}

int PS2Keyboard::readUnicode() {
  int result;

  result = CharBuffer;
  if (!result) result = get_iso8859_code();
  if (!result) return -1;
  UTF8next = 0;
  CharBuffer = 0;
  return result;
}

PS2Keyboard::PS2Keyboard() {
  // nothing to do here, begin() does it all
}

void PS2Keyboard::begin( const PS2Keymap_t &map) {
 
//  DataPin = data_pin;
  keymap = &map;

  // initialize the pins
#ifdef INPUT_PULLUP
  pinMode(clk, INPUT_PULLUP);
  pinMode(daat, INPUT_PULLUP);
#else
  pinMode(clk, INPUT);
  digitalWrite(clk, HIGH);
  pinMode(daat, INPUT);
  digitalWrite(daat, HIGH);
#endif


  head = 0;
  tail = 0;

//    attachInterrupt(clk, ps2interrupt, FALLING);
esp_err_t err= gpio_isr_handler_add(GPIO_NUM_2, &ps2interrupt,(void*) 2);
//if 
err= gpio_set_intr_type(GPIO_NUM_2,GPIO_INTR_NEGEDGE);
  Serial.println("working here");delay(3000);

}




//const int DataP = D2;
//const int IRQp =  D1;
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#ifndef FDB_LIQUID_CRYSTAL_I2C_H
#define FDB_LIQUID_CRYSTAL_I2C_H

#include <inttypes.h>
#include <Print.h>



// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En B00000100  // Enable bit
#define Rw B00000010  // Read/Write bit
#define Rs B00000001  // Register select bit

/**
 * This is the driver for the Liquid Crystal LCD displays that use the I2C bus.
 *
 * After creating an instance of this class, first call begin() before anything else.
 * The backlight is on by default, since that is the most likely operating mode in
 * most cases.
 */
class LiquidCrystal_I2C : public Print {
public:
   /**
    * Constructor
    *
    * @param lcd_addr   I2C slave address of the LCD display. Most likely printed on the
    *               LCD circuit board, or look in the supplied LCD documentation.
    * @param lcd_cols   Number of columns your LCD display has.
    * @param lcd_rows   Number of rows your LCD display has.
    * @param charsize   The size in dots that the display has, use LCD_5x10DOTS or LCD_5x8DOTS.
    */
   LiquidCrystal_I2C(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize = LCD_5x8DOTS);

   /**
    * Set the LCD display in the correct begin state, must be called before anything else is done.
    */
   void begin(uint8_t sda, uint8_t scl);
   
    /**
     * Remove all the characters currently shown. Next print/write operation will start
     * from the first position on LCD display.
     */
   void clear();
     
   /**
    * Next 
/write operation will will start from the first position on the LCD display.
    */
   void home();

    /**
     * Do not show any characters on the LCD display. Backlight state will remain unchanged.
     * Also all characters written on the display will return, when the display in enabled again.
     */
   void noDisplay();
     
   /**
    * Show the characters on the LCD display, this is the normal behaviour. This method should
    * only be used after noDisplay() has been used.
    */ 
   void display();

   /**
    * Do not blink the cursor indicator.
    */
   void noBlink();
    
   /**
    * Start blinking the cursor indicator.
    */ 
   void blink();    

   /**
    * Do not show a cursor indicator.
    */
   void noCursor();

   /**
     * Show a cursor indicator, cursor can blink on not blink. Use the
    * methods blink() and noBlink() for changing cursor blink.
    */ 
   void cursor();

   void scrollDisplayLeft();
   void scrollDisplayRight();
   void printLeft();
   void printRight();
   void leftToRight();
   void rightToLeft();
   void shiftIncrement();
   void shiftDecrement();
   void noBacklight();
   void backlight();
   void autoscroll();
   void noAutoscroll(); 
   void createChar(uint8_t, uint8_t[]);
   void setCursor(uint8_t, uint8_t); 
   virtual size_t write(uint8_t);
   void command(uint8_t);

   inline void blink_on() { blink(); }
   inline void blink_off() { noBlink(); }
   inline void cursor_on() { cursor(); }
   inline void cursor_off() { noCursor(); }

// Compatibility API function aliases
   void setBacklight(uint8_t new_val);            // alias for backlight() and nobacklight()
   void load_custom_character(uint8_t char_num, uint8_t *rows);   // alias for createChar()
   void printstr(const char[]);
    
private:
   void send(uint8_t, uint8_t);
   void write4bits(uint8_t);
   void expanderWrite(uint8_t);
   void pulseEnable(uint8_t);
   uint8_t _addr;
   uint8_t _displayfunction;
   uint8_t _displaycontrol;
   uint8_t _displaymode;
   uint8_t _cols;
   uint8_t _rows;
   uint8_t _charsize;
   uint8_t _backlightval;
};

#endif // FDB_LIQUID_CRYSTAL_I2C_H



//#include "LiquidCrystal_I2C.h"
#include <inttypes.h>
#include <Arduino.h>
#include <Wire.h>



LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize)
{
  _addr = lcd_addr;
  _cols = lcd_cols;
  _rows = lcd_rows;

void  st_result(){
 sss:clr();
 lcd.setCursor(1,1);lcd.print("1)% ATTDANCE FILTER"); 
 lcd.setCursor(1,2);lcd.print("2)SEND ALL STU.DATA"); 
 lcd.setCursor(1,3);lcd.print("3)SEND ALL LEC DATA"); 
                                                                                  
  while(1){
 k=keypd();  
 
 if(k=='1') {print_percentage();goto sss;}     
 if(k=='2') {print_all_stu();goto sss;}
 if(k=='3') {print_all_lec();goto sss;}
 if(k==205) {return;} 
  }
}  
void  sett_time(){edit_time();}         
void  sett_pass(){edit_pass();}
//void  sett_blue(){edit_bluetooth();}
//void  sett_log(){logging();}





void  st_settings(){     
 sss:clr();
 lcd.setCursor(1,1);lcd.print("1)SET TIME & DATE");  
 lcd.setCursor(1,2);lcd.print("2)CHANGE PASSWORD"); 
 lcd.setCursor(1,3);lcd.print("3)WIFI SETTINGS"); 
 lcd.setCursor(1,4);lcd.print("4)FACTORY RESET "); 
 //lcd.setCursor(13,1);lcd.print(""); 

  
  while(1){
 k= keypd();  
 if(k=='1') {sett_time();goto sss;}  
 if(k=='2') {sett_pass();goto sss;}
  if(k=='3') {edit_wifi();goto sss;}
  if(k=='4') {reset();goto sss;}
 if(k==205) {return;} 
  }
}

void  menu(){

 if(password8()==1){} 
 else {return;}

 sss:clr();   
     
 lcd.setCursor(1,1);lcd.print("1)STUDENTS DATA");  
 lcd.setCursor(1,2);lcd.print("2)LECTURE DATA"); 
// lcd.setCursor(1,3);lcd.print("3)EXPORT DATA"); 
 lcd.setCursor(1,3);lcd.print("3)SYSTEM SETTINGS"); 

  while(1){
k= keypd();             
 if(k=='1') {st_student();goto sss;}  
 if(k=='2') {st_lecture();goto sss;}
// if(k=='3') {st_result();goto sss;}
 if(k=='3') {st_settings();goto sss;}

 if(k==205) {return;} 
  }
                                                                                  
}     
                                                                                     



uint8_t current_class;

uint8_t m=1;
char fpbuffer[20]={};
                                                                                                                           
void clear_buf(){
  Serial2.flush();
 while (Serial2.available()>0){char t=Serial2.read();}
  delay(100);
fpbuffer[0]=0xff;  
fpbuffer[1]=0xff;
fpbuffer[2]=0xff;
fpbuffer[3]=0xff;
fpbuffer[4]=0xff;
fpbuffer[5]=0xff;
fpbuffer[6]=0xff;
fpbuffer[7]=0xff;
fpbuffer[8]=0xff;
fpbuffer[9]=0xff;
fpbuffer[10]=0xff;
fpbuffer[11]=0xff;//
fpbuffer[12]=0xff;
fpbuffer[13]=0xff;
fpbuffer[14]=0xff;
fpbuffer[15]=0xff;
} 






void  print_student(uint16_t  st){           
uint8_t bb;
  
     clr();
    lcd.setCursor(1,2); lcd.print("Printing Student"); 
    lcd.setCursor(1,3); lcd.print("data..Please wait!");  
     
sprintf( btt,"STUDENT SERIAL NO:%02u",st);SerialBT.print(btt);   sprintf( btt,"\r\n");SerialBT.print(btt);  
sprintf( btt,"MATRIC NO:");SerialBT.print(btt); for(bb=0;bb<=19;bb++){SerialBT.write(EEPROM.read(st*100+10+bb));}    sprintf( btt,"\r\n");SerialBT.print(btt);
sprintf( btt,"NAME:----:");SerialBT.print(btt);      for(bb=0;bb<=19;bb++){SerialBT.write(EEPROM.read(st*100+30+bb));}    sprintf( btt,"\r\n");SerialBT.print(btt);
sprintf( btt,"PHONE NO.:");SerialBT.print(btt);     for(bb=0;bb<=13;bb++){SerialBT.write(EEPROM.read(st*100+60+bb));}    sprintf( btt,"\r\n");SerialBT.print(btt); delay(100);
sprintf( btt,"SEX:-----:");SerialBT.print(btt);       for(bb=0;bb<=5;bb++){SerialBT.write(EEPROM.read(st*100+74+bb));}    sprintf( btt,"\r\n");SerialBT.print(btt);
sprintf( btt,"RFID:-:"); SerialBT.print(btt);  for(bb=0;bb<=19;bb++){SerialBT.write(EEPROM.read(st*100+80+bb));}    sprintf( btt,"\r\n");SerialBT.print(btt); 
sprintf( btt,"LAST SEEN:%02u:%02u:%02u %02u/%02u/20%02u",
EEPROM.read(st*100+2),
EEPROM.read(st*100+3),
EEPROM.read(st*100+4),
EEPROM.read(st*100+5),                                                                                                
EEPROM.read(st*100+6),                                                                       
EEPROM.read(st*100+7)

);  SerialBT.print(btt);          
  

sprintf( btt,"\r\n");SerialBT.print(btt); sprintf( btt,"\r\n");SerialBT.print(btt);  delay(2000); 

sprintf( btt,"LECTURES ATTENDED:\r\n"); SerialBT.print(btt); 
 
uint8_t t=1;
for(uint16_t  w=1;w<=19;w++){

if(EEPROM.read(2000+w*100+st)>0){
sprintf( btt,"(%u)",t);SerialBT.print(btt);t++;
sprintf( btt,"Lec:%02u ",w);SerialBT.print(btt); for(bb=0;bb<=19;bb++){SerialBT.write(EEPROM.read(2000+w*100+10+bb));}    sprintf( btt,"\r\n");SerialBT.print(btt);
}

}

if(t==1) sprintf( btt,"No Lecture Attended\r\n");SerialBT.print(btt);
sprintf( btt,"\r\n");SerialBT.print(btt);sprintf( btt,"\r\n");SerialBT.print(btt);sprintf( btt,"\r\n");SerialBT.print(btt);sprintf( btt,"\r\n");SerialBT.print(btt);
delay(2000);                   

}///prints the student details and all the lectures he/she has attended with position of attendance

void  print_lecture(uint16_t  st){
uint8_t bb;
    
     clr();
    lcd.setCursor(1,2); lcd.print("Printing Lecture"); 
    lcd.setCursor(1,3); lcd.print("data..Please wait!"); 
    sprintf( btt,"\r\n"); sprintf( btt,"\r\n");// sprintf( btt,"\r\n"); sprintf( btt,"\r\n"); sprintf( btt,"\r\n");  
     
sprintf( btt,"LECTURE SERIAL NO-:%03u",EEPROM.read(2000+st*100+1));SerialBT.print(btt);   sprintf( btt,"\r\n"); SerialBT.print(btt);  
sprintf( btt,"STARTED---:%02u:%02u:%02u %02u/%02u/20%02u",
EEPROM.read(2000+st*100+2),
EEPROM.read(2000+st*100+3),
EEPROM.read(2000+st*100+4),
EEPROM.read(2000+st*100+5),
EEPROM.read(2000+st*100+6),
EEPROM.read(2000+st*100+7)

);SerialBT.print(btt);sprintf( btt,"\r\n");SerialBT.print(btt);
  
sprintf( btt,"COURSE----:");SerialBT.print(btt); for(bb=0;bb<=29;bb++){SerialBT.write(EEPROM.read(2000+st*100+10+bb));}    sprintf( btt,"\r\n");SerialBT.print(btt);
sprintf( btt,"LECTURER--:");SerialBT.print(btt);      for(bb=0;bb<=29;bb++){SerialBT.write(EEPROM.read(2000+st*100+40+bb));}    sprintf( btt,"\r\n");SerialBT.print(btt);
//sprintf( btt,"REMARKS---:");     for(bb=0;bb<=29;bb++){SerialBT.write(EEPROM.read(2000+st*100+70+bb));}    sprintf( btt,"\r\n");
delay(2000);                         
sprintf( btt,"STUDENTS ATTENDANCE\r\n"); SerialBT.print(btt);
sprintf( btt,"     SN    MATRIC       NAME   \r\n"); SerialBT.print(btt);
uint8_t t=1;                                 
for(uint16_t  w=1;w<=20;w++){


if(EEPROM.read(2000+st*100+80+w)>0){
sprintf( btt,"(%u)-",t);SerialBT.print(btt);t++;

sprintf( btt,"%02u-",EEPROM.read(w*100+1)); SerialBT.print(btt);
//SerialBT.write(EEPROM.read(w*100+80));SerialBT.write(EEPROM.read(w*100+81));  sprintf( btt,"-");// remarks  
 for(bb=0;bb<=9;bb++){SerialBT.write(EEPROM.read(w*100+10+bb));}    //MATRIC  
//  
 sprintf( btt,"\r\n");SerialBT.print(btt);
    sprintf( btt,"   ");  SerialBT.print(btt);       
    for(bb=0;bb<=27;bb++){SerialBT.write(EEPROM.read(w*100+30+bb));}    //NAME
sprintf( btt,"\r\n");SerialBT.print(btt);  delay(300); 
}

}
 if(t==1) sprintf( btt,"No Student Attended\r\n");SerialBT.print(btt);
sprintf( btt,"\r\n");SerialBT.print(btt);sprintf( btt,"\r\n");SerialBT.print(btt);sprintf( btt,"\r\n");SerialBT.print(btt);sprintf( btt,"\r\n"); SerialBT.print(btt);
//delay(2000);
                                        
 
}// prints lecture details and list the student that came
int ll; 
void  printt(){  

uint16_t  bb;