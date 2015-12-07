/*
 Demonstrate 0.96 in. OLED module (128x64/SPI). Revised for use with ST Nucleo L152RE

 Note:  On 16 pin OLED module from Taobao, MOSI connects to module D1
        and CLK (also called SCK) connects to module D0. Module RW and RD lines
        are connected to GND.
*/

#include "mbed.h"
#include "PinDetect.h"
#include "ssd1306.h"

/* definitions for ST Nucleo L152RE  */
/*
#define RED_Pin GPIO_PIN_3
#define RED_GPIO_Port GPIOB
#define YELLOW_Pin GPIO_PIN_4
#define YELLOW_GPIO_Port GPIOB
#define GREEN_Pin GPIO_PIN_5
#define GREEN_GPIO_Port GPIOB
*/

#define LED_RED PB_3
#define LED_GREEN PB_4
#define LED_BLUE PB_5

#define CSEL  PC_15  // CS  D10
#define RST   PB_8  //     D9
#define DCMD  PB_9  // DC  D8
#define CLK   PC_10  // CLK D13
#define DATA  PC_12  // MOSI D11

#define BLUEFRUIT_SPI_CS               PB_10
#define BLUEFRUIT_SPI_IRQ              PA_8
#define BLUEFRUIT_SPI_RST              PA_10
#define BLUEFRUIT_SPI_SCK              PB_3
#define BLUEFRUIT_SPI_MISO             PB_4
#define BLUEFRUIT_SPI_MOSI             PB_5

void render();
void displayFunction( void );
void changeSelection();
void select();
// SSD1306 oled(p8 /* cs */, p9 /* reset */, p14 /* dc */, p13 /* clock */, p11 /* data */); // LPC1768
// SSD1306 oled(PTA13 /* cs */, PTD5 /* reset */, PTD0 /* dc */, PTD1 /* clock */, PTD2 /* data */); // KL25Z
// SSD1306 oled(D10 /* cs */, D9 /* reset */, D8 /* dc */, D13 /* clock */, D11 /* data */); // KL05Z or Arduino styles
SSD1306 oled(CSEL, RST, DCMD, CLK, DATA);   // STM32 Nucleo
DigitalOut _led_r(LED_RED), _led_g(LED_GREEN), _led_b(LED_BLUE);
 

volatile int cScreen = 1; //Current screen state
volatile int hours = 9;
volatile int minutes = 26;
volatile int seconds = 0;
volatile int notifyFlag = 0;
Ticker flipper;
PinDetect pb(PB_1); //Select
PinDetect pb1(PA_10); //Back
PinDetect pb2(PB_2); //Up
PinDetect pb3(PB_0);//Down
volatile int selection = 0;
volatile int colorChange = 0; //0 is BG 1 is Font
volatile uint16_t bgColor = oled.Color565(0,0,0);
volatile uint16_t textColor = oled.Color565(255,255,255);

volatile char sTime[8];
volatile char notifyMessage[64];
volatile int prevSeconds = 0;
//Screen 0 = main

//Guidelines for screen
//Top bar is 0-128x and 0-20y
//battery is 100-126x and 3-18y
//String is within x:10 to x:128 and y is y:35 to y:55
//Boxes are 5-125x and from 25y-45y, 50y-70y, etc.



void render() {
    prevSeconds = -1;
    //HEADER
    oled.fillRect(0,0,128,20,oled.ColorHSV(1200,0,100));
    oled.fillRect(0,20,128,108,bgColor);
    //BATTERY
    oled.fillRect(100,3,24,15,oled.ColorHSV(600,255,255));
    oled.fillRect(124,6,2,9,oled.ColorHSV(600,255,255));
    oled.drawFastHLine(100,3,24,oled.Color565(0,0,0));
    oled.drawFastHLine(100,18,24,oled.Color565(0,0,0));
    oled.drawFastVLine(100,3,15,oled.Color565(0,0,0));
    oled.drawFastVLine(123,3,15,oled.Color565(0,0,0));
    oled.fillRect(124,6,2,9,oled.Color565(0,0,0));
    //Individual pages
    if(cScreen == 0) {
        selection = 0;
        if(notifyFlag == 1){ 
            oled.fillRect(3,110,122,18,oled.Color565(50,50,50));
            oled.drawString(5,113, (char*)notifyMessage, oled.Color565(255,255,255), oled.Color565(255,255,255),1);    
        }
        else {
            oled.fillRect(3,110,122,18,bgColor);
        }
    }
    else if(cScreen == 1) {
        selection = 0;
        //Individual boxes are from 5-123x and are 20px high  5px buffer
        oled.fillRect(5,25,118,20,oled.Color565(80,150,80));
        oled.fillRect(5,50,118,20,oled.ColorHSV(800,0,100));
        oled.drawString(45,32, "Change BG", oled.Color565(255,255,255), oled.Color565(255,255,255),1);
        oled.drawString(37,57, "Change Font", oled.Color565(255,255,255), oled.Color565(255,255,255),1);
    }
    else if(cScreen == 2) { //Color Selection
        selection = 0;
        oled.fillRect(5,25,118,20,oled.Color565(80,150,80));
        oled.fillRect(5,50,118,20,oled.ColorHSV(800,0,100));      
        oled.fillRect(5,75,118,20,oled.ColorHSV(800,0,100));  
        oled.fillRect(5,100,118,20,oled.ColorHSV(800,0,100));   
        oled.fillRect(5,125,118,4,oled.ColorHSV(800,0,100));  
        oled.drawString(50,32, "Black", oled.Color565(255,255,255), oled.Color565(255,255,255),1);
        oled.drawString(50,57, "White", oled.Color565(255,255,255), oled.Color565(255,255,255),1);
        oled.drawString(50,82, "Red", oled.Color565(255,255,255), oled.Color565(255,255,255),1);           
        oled.drawString(50,107, "Blue", oled.Color565(255,255,255), oled.Color565(255,255,255),1);    
    }
}

void changeSelection() {
    int i;
    int base = 0;
    int range = (cScreen == 1)? 2 : 6;
    if(selection >= range) selection = range - 1;
    else if(selection < 0) selection = 0;
    if(selection > 3) base = 4;
    if(cScreen != 0) {
        for(i = base; i < range; i++) {
            oled.fillRect(5,25*((i%4)+1),118,20,oled.ColorHSV(800,0,100));    
        }
        if(range - base < 4) {
            for(i = (range-base); i <= 6-(range-base); i++) {
                oled.fillRect(5,25*(i+1),118,20,bgColor);     
            }
        }
        oled.fillRect(5,25*((selection%4) + 1),118,20,oled.Color565(80,150,80));
        if(cScreen == 1) {
            oled.drawString(45,32, "Change BG", oled.Color565(255,255,255), oled.Color565(255,255,255),1);
            oled.drawString(37,57, "Change Font", oled.Color565(255,255,255), oled.Color565(255,255,255),1);       
        }
        else if(cScreen == 2) {
            if(base == 0) {
                oled.drawString(50,32, "Black", oled.Color565(255,255,255), oled.Color565(255,255,255),1);
                oled.drawString(50,57, "White", oled.Color565(255,255,255), oled.Color565(255,255,255),1);
                oled.drawString(50,82, "Red", oled.Color565(255,255,255), oled.Color565(255,255,255),1); 
                oled.drawString(50,107, "Blue", oled.Color565(255,255,255), oled.Color565(255,255,255),1);                   
            }
            else { 
                oled.drawString(50,32, "Green", oled.Color565(255,255,255), oled.Color565(255,255,255),1);
                oled.drawString(50,57, "Yellow", oled.Color565(255,255,255), oled.Color565(255,255,255),1);                    
            }
        }            
    }
}

void notification() {
    notifyFlag = 1;
    render();
}

void select() {
    if(cScreen == 0) {
        cScreen = 1;
        render();
    }
    else if (cScreen == 1){
        if(selection == 0) { 
            colorChange = 0;
        }
        else {
            colorChange = 1;
        }
        cScreen = 2;
        render();
    }
    else if (cScreen == 2){
        if(selection == 0) {
            if(colorChange == 0) bgColor = oled.Color565(0,0,0);
            else textColor = oled.Color565(0,0,0);
        }
        else if(selection == 1) {
            if(colorChange == 0) bgColor = oled.Color565(255,255,255);
            else textColor = oled.Color565(255,255,255);         
        }
        else if(selection == 2) {
            if(colorChange == 0) bgColor =  oled.Color565(255,0,0);
            else textColor = oled.Color565(255,0,0);             
        }
        else if(selection == 3) {
            if(colorChange == 0) bgColor =  oled.Color565(0,0,255);
            else textColor = oled.Color565(0,0,255);             
        }
        else if(selection == 4) {
            if(colorChange == 0) bgColor =  oled.Color565(0,255,0);
            else textColor = oled.Color565(0,255,0);           
        }
        else if(selection == 5) {
            if(colorChange == 0) bgColor =  oled.Color565(255,255,0);
            else textColor = oled.Color565(255,255,0);            
        }
        cScreen = 0;
        render();
    }
}

void pb_hit_interrupt (void) {
    select();
}

void pb1_hit_interrupt (void) {
    if(cScreen == 0) {
        notifyFlag = 0;    
    }
    else {
        cScreen--;
    }
    render();
}

void pb2_hit_interrupt (void) { //Up
    if(cScreen == 0) {
        notifyFlag = 1;   
        render(); 
    }
    else {
        selection--;
        changeSelection();        
    }
}

void pb3_hit_interrupt (void) { //Down
    selection++;
    changeSelection();
}

void displayFunction( void )
{
    //Clear portion
    if(cScreen == 0) {
        sprintf((char*) sTime, "%.2d:%.2d:%.2d", hours, minutes, seconds/4);
        if(prevSeconds != (seconds/4)) {
            oled.fillRect(10,35,118,20,bgColor);
            oled.drawString(10,35, (char*) sTime, textColor, textColor,2);           
        }
        prevSeconds = seconds/4;
    }
    //Time logic
    seconds = seconds + 1;
    if(seconds >= 60*4) {
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
    //Initializations for Pushbuttons
    pb.mode(PullUp);
    pb1.mode(PullUp);
    pb2.mode(PullUp);
    pb3.mode(PullUp);
    
    wait(.001);
    
    pb.attach_deasserted(&pb_hit_interrupt);
    pb.setSampleFrequency();
    pb1.attach_deasserted(&pb1_hit_interrupt);
    pb1.setSampleFrequency();
    pb2.attach_deasserted(&pb2_hit_interrupt);
    pb2.setSampleFrequency();
    pb3.attach_deasserted(&pb3_hit_interrupt);
    pb3.setSampleFrequency();
    
    //LED
    _led_r = 1;
    _led_g = 1;
    _led_b = 1;
    
    //Initializations for OLED
    oled.begin();
    oled.setWrap(false);
    set_time(1256729737); // Set time to Wed, 28 Oct 2009 11:35:37
    
    //Initializations for Timer interrupt
    flipper.attach(&displayFunction, 0.25);
    render();
    sprintf((char*)notifyMessage, "Test Notification");
    notification();
    // oled.fillScreen(oled.ColorHSV(0,255,255));      
    // oled.drawString(x, y, name,oled.Color565(125,255,255),oled.Color565(125,255,255),1);
    while(1) {

    }
}

// EOF