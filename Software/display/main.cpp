/*
 Demonstrate 0.96 in. OLED module (128x64/SPI). Revised for use with ST Nucleo L152RE

 Note:  On 16 pin OLED module from Taobao, MOSI connects to module D1
        and CLK (also called SCK) connects to module D0. Module RW and RD lines
        are connected to GND.
*/

#include "mbed.h"

#include "ssd1306.h"

/* definitions for ST Nucleo L152RE  */
#define CSEL  PB_6  // CS  D10
#define RST   PC_7  //     D9
#define DCMD  PA_9  // DC  D8
#define CLK   PA_5  // CLK D13
#define DATA  PA_7  // MOSI D11

#define BLUEFRUIT_SPI_CS               PB_10
#define BLUEFRUIT_SPI_IRQ              PA_8
#define BLUEFRUIT_SPI_RST              PA_10

#define BLUEFRUIT_SPI_SCK              PB_3
#define BLUEFRUIT_SPI_MISO             PB_4
#define BLUEFRUIT_SPI_MOSI             PB_5

void render();
void displayFunction( void );
// SSD1306 oled(p8 /* cs */, p9 /* reset */, p14 /* dc */, p13 /* clock */, p11 /* data */); // LPC1768
// SSD1306 oled(PTA13 /* cs */, PTD5 /* reset */, PTD0 /* dc */, PTD1 /* clock */, PTD2 /* data */); // KL25Z
// SSD1306 oled(D10 /* cs */, D9 /* reset */, D8 /* dc */, D13 /* clock */, D11 /* data */); // KL05Z or Arduino styles
SSD1306 oled(CSEL, RST, DCMD, CLK, DATA);   // STM32 Nucleo

int cScreen = 0; //Current screen state
int hours = 0;
int minutes = 0;
int seconds = 0;
Ticker flipper;
//Screen 0 = main
void render() {
    if(cScreen == 0) {
            oled.fillRect(0,0,128,20,oled.ColorHSV(1200,255,255));
            oled.fillRect(0,20,128,108,oled.Color565(0,0,0));
    }
}

void displayFunction( void )
{
    oled.fillRect(10,35,118,20,oled.Color565(0,0,0));
    char sTime[8];
    sprintf(sTime, "%.2d:%.2d:%.2d", hours, minutes, seconds);
    
    if(cScreen == 0) {
        oled.drawString(10,35, sTime, oled.Color565(255,255,255), oled.Color565(255,255,255),2);
         //oled.drawString(0,35, "Testing", oled.Color565(255,255,255), oled.Color565(255,255,255),1);
    }
    seconds = seconds + 1;
    if(seconds >= 60) {
        seconds = 0;
        minutes += 1;
        if(minutes >= 60) {
            minutes = 0;
            hours += 1;
            if(hours >= 24) {
                hours = 0;    
            }    
        }    
    }
}

int main()
{
    oled.begin();
    oled.setWrap(false);
    render();
    set_time(1256729737); // Set time to Wed, 28 Oct 2009 11:35:37
    flipper.attach(&displayFunction, 1.0);
    // oled.fillScreen(oled.ColorHSV(0,255,255));      
    // oled.drawString(x, y, name,oled.Color565(125,255,255),oled.Color565(125,255,255),1);
    while(1) {
        /*oled.fillScreen(oled.ColorHSV(0,255,255)); 
        oled.drawString(x, y, name,oled.Color565(125,255,255),oled.Color565(125,255,255),1);
        x += 2;
        if(x == 64) {
            y += 1;   
        }
        else if (x >= 128) {
            x = 0;   
        }        
        wait(0.3);
        */
        /*oled.fillRect(x,y,1,1,oled.ColorHSV(0,255,255));
        if (y == 15 && x == 0) {
                
        }
        if(x > 128) {
            x = 0;
            y += 1;
            if(y > 128) {
                y = 0;
                oled.fillScreen(oled.ColorHSV(700,255,125));  
            }
        } else {
            x += 1;
        }*/
        //wait(0.1);

    } // end outside loop for OLED demo
}

// EOF