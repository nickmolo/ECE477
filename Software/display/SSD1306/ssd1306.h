#ifndef __SSD1306_H__
#define __SSD1306_H__


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

struct FontDescriptor
{
    uint8_t width;      // width in bits
    uint8_t height;     // char height in bits
    uint16_t offset;    // offset of char into char array
};
/** SSD1306 Controller Driver
  *
  * This class provides a buffered display for the SSD1306 OLED controller.
  *
  * TODO:
  *   - At the moment, the driver assumes a 128x64 pixel display.
  *   - Only fonts of 8 pixel height are supported (different widths can be used).
  *   - Pretty much no drawing functions are provided as yet.
  *   - Possible "auto-update", automatically calling update() after a printf etc.
  *
  * Information taken from the datasheet at:
  *   http://www.adafruit.com/datasheets/SSD1306.pdf
  *
  */
class SSD1306
{
public:
    /** Construct a new SSD1306 object.
     *  @param cs The connected C/S pin.
     *  @param rs The connected RS pin.
     *  @param dc The connected DC pin.
     *  @param clk The connected CLK pin.
     *  @param data The connected Data pin.
     */
    SSD1306(PinName cs, PinName rs, PinName dc, PinName clk, PinName data);

    // ----- HARDWARE CONTROL -----

    uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);
    uint16_t ColorHSV(long hue, uint8_t sat, uint8_t val);
    // drawing primitives!
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void fillRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void fillScreen(uint16_t fillcolor);

    void invert(bool);
    // commands
    void begin(void);
    void goTo(int x, int y);

    void reset(void);

    /* low level */

    void writeData(uint8_t d);
    void writeCommand(uint8_t c);


    void writeData_unsafe(uint16_t d);

    void setWriteDir(void);
    void write8(uint8_t d);
    void drawChar(int16_t x, int16_t y, unsigned char c,
                uint16_t color, uint16_t bg, uint8_t size);
    void drawString(int16_t x, int16_t y, char * c,
                uint16_t color, uint16_t bg, uint8_t size);
    void setWrap(bool newWrap);
    
private:
    SPI _spi;
    void spiwrite(unsigned char value);
    DigitalOut _cs, _reset, _dc;
    bool wrap;
    void _send_command(unsigned char code);
    void _send_data(unsigned char value);

};

#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 128

#endif
