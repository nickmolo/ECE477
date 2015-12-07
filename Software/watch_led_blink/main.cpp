#include "mbed.h"

DigitalOut red(PB_3);
DigitalOut yellow(PB_4);
DigitalOut green(PB_5);

int main() {
    while(1) {
        red = 1;
        yellow = 1;
        green = 1;
        wait(1);
        red = 0;
        yellow =0;
        green = 0;
        wait(2);
    }
}
