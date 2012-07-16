#include <htc.h>
#include "hardware.h"
#include "led.h"

#define LED_SHORT_DELAY     10
#define LED_LONG_DELAY      50

led_mode_type led_mode;
unsigned char led_state;
unsigned char led_counter;


void led_init()
{

    // Enable timer functionality on Timer0
    T0CS = 0;

    // Assign prescalar to the timer
    PSA = 0;

    // Set the prescalar to 256
    OPTION_REGbits.PS = 4;

    // Enable Timer 0 interrupt
    INTCONbits.TMR0IE = 1;

    // Initially, turn led off
    led_state = 1;
    led_counter = 0;

    // Set the LED pin to output mode
    LED_TRIS = 0;
}

void led_set_mode(led_mode_type newmode)
{
    led_mode = newmode;
}

void led_interrupt_handler()
{
    
    switch(led_mode) {
        case ON:
            led_state = 0;
            break;

        case OFF:
            led_state = 1;
            break;

        case SLOW:
            if (++led_counter > LED_LONG_DELAY) {
                led_counter = 0;
                led_state = !led_state;
            }
            break;

        case FAST:
            if (++led_counter > LED_SHORT_DELAY) {
                led_counter = 0;
                led_state = !led_state;
            }
            break;
    }

    LED_GPIO = led_state;
     
}

