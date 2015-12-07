#include "stm32l4xx_hal.h"
#include "spi.h"
#include "mxconstants.h" //remove if unecessary
#include <stdarg.h>

#define bool int
#define true 1
#define false 0



#define SSD1351_CMD_SETCOLUMN       0x15
#define SSD1351_CMD_SETROW          0x75
#define SSD1351_CMD_WRITERAM        0x5C
#define SSD1351_CMD_READRAM         0x5D
#define SSD1351_CMD_SETREMAP        0xA0
#define SSD1351_CMD_STARTLINE       0xA1
#define SSD1351_CMD_DISPLAYOFFSET   0xA2
#define SSD1351_CMD_DISPLAYALLOFF   0xA4
#define SSD1351_CMD_DISPLAYALLON    0xA5
#define SSD1351_CMD_NORMALDISPLAY   0xA6
#define SSD1351_CMD_INVERTDISPLAY   0xA7
#define SSD1351_CMD_FUNCTIONSELECT  0xAB
#define SSD1351_CMD_DISPLAYOFF      0xAE
#define SSD1351_CMD_DISPLAYON       0xAF
#define SSD1351_CMD_PRECHARGE       0xB1
#define SSD1351_CMD_DISPLAYENHANCE  0xB2
#define SSD1351_CMD_CLOCKDIV        0xB3
#define SSD1351_CMD_SETVSL      0xB4
#define SSD1351_CMD_SETGPIO         0xB5
#define SSD1351_CMD_PRECHARGE2      0xB6
#define SSD1351_CMD_SETGRAY         0xB8
#define SSD1351_CMD_USELUT      0xB9
#define SSD1351_CMD_PRECHARGELEVEL  0xBB
#define SSD1351_CMD_VCOMH       0xBE
#define SSD1351_CMD_CONTRASTABC     0xC1
#define SSD1351_CMD_CONTRASTMASTER  0xC7
#define SSD1351_CMD_MUXRATIO            0xCA
#define SSD1351_CMD_COMMANDLOCK         0xFD
#define SSD1351_CMD_HORIZSCROLL         0x96
#define SSD1351_CMD_STOPSCROLL          0x9E
#define SSD1351_CMD_STARTSCROLL         0x9F

#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 128

struct FontDescriptor
{
    uint8_t width;      // width in bits
    uint8_t height;     // char height in bits
    uint16_t offset;    // offset of char into char array
};

inline void spiwrite(unsigned char value);
void writeCommand(uint8_t c);
void writeData(uint8_t c);
void goTo(int x, int y);
uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);
void fillScreen(uint16_t fillcolor);
void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t fillcolor);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void drawPixel(int16_t x, int16_t y, uint16_t color);
void begin(void);
void invert(bool v);
uint16_t ColorHSV(long hue, uint8_t sat, uint8_t val);
void setWrap(bool newWrap);
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
void drawString(int16_t x, int16_t y, char * c, uint16_t color, uint16_t bg, uint8_t size);