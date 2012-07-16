/* 
 * File:   main.c
 * Author: avishay
 *
 * Created on June 28, 2012, 12:02 AM
 */

#include <htc.h>
#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "led.h"
#include "load_pwm.h"

// Configure the controller for internal oscillator operatin
// of 4MHz and no watchdog timer
__CONFIG(FOSC_INTOSCIO & WDTE_OFF & IOSCFS_4MHZ);

#define MID_POINT   512
#define GUARD_BAND  60
#define ZERO_STABILITY   50


#define STATE_ZEROING   0
#define STATE_BIDIR     1
#define STATE_UNIDIR    2

void delay()
{
    unsigned char i;
    for(i=0; i < 34; i++);
}
void putch(char c)
{
    unsigned char j;

    GP1 = 0; // Start bit
    delay();

    // Data bits
    for(j=0; j < 8; j++) {
            GP1 = c;
            c >>= 1;
            delay();
    }

    // Stop bit
    GP1 = 1;
    delay();

//    for(j=0; j < 3; j++)
//            delay();

}

void putnum(int n)
{
    if (n >= 2000) {
        putch('*');
    }
    else
        if (n > 1000) {
            putch('1');
            n -= 1000;
        }
    putch(n / 100 + '0');
    n -= (n / 100)*100;
    putch(n / 10 + '0');
    n -= (n / 10)*10;
    putch(n + '0');
    putch('\n');
    putch('\r');
}

unsigned char is_low_zero(int n, int zero)
{
    // Low point zero
    if (n <= GUARD_BAND+zero)
        return 1;
    else
        return 0;
}

unsigned char is_mid_zero(int n, int zero)
{
    // Mid-point zero
    if ((n >= (zero-GUARD_BAND)) && (n <= (zero+GUARD_BAND)))
        return 1;
    else
        return 0;
}

void short_delay()
{
    unsigned char i, k;
    for(k=0; k < 90; k++)
        for(i=0; i < 250; i++);
}

void mark_unidirectional()
{
    led_set_mode(OFF);
    short_delay();
    led_set_mode(ON);
    short_delay();
    led_set_mode(OFF);
}

void mark_bidirectional()
{
    unsigned char j;
    for(j=0; j < 3; j++) {
        led_set_mode(OFF);
        short_delay();
        led_set_mode(ON);
        short_delay();
    }
    led_set_mode(OFF);
}


/*
 * 
 */
void main() {
    unsigned char j;

    // Initialization
    led_init();
    pwm_init();

    led_set_mode(ON);
    pwm_set(0);

    // Enable interrupts
    INTCONbits.GIE = 1;

    // Timer 1 setup (1MHz operation)
    T1CONbits.TMR1CS = 0; // Set internal clock source
    T1CONbits.T1CKPS = 0; // 1:1 Prescalar
    CMCON1bits.T1ACS = 0; // Clock is instruction clock
    T1CONbits.TMR1ON = 0; // Turn the timer off

    // Setup the input pin
    RX_ANSEL = 0;         // Digital input
    RX_TRIS = 1;          // Input direction

    unsigned char inpsr = 0;
    char rc_state = 0;
    int stop = 0;
    unsigned int avbuf[8];
    char avbuf_index = 0;
    unsigned int average;
    unsigned char state = STATE_ZEROING;
    unsigned char stability = ZERO_STABILITY;
    int z;
    int zero;

    while(1) {
        // Shift an input bit from the input the
        // shift register
        inpsr = (inpsr << 1) | RX_GPIO;

        // Check if a state change occured
        if (rc_state == 1) {
            if (inpsr == 0) {
                rc_state = 0;

                // Stop Timer 1 and record its value
                T1CONbits.TMR1ON = 0;
                stop = TMR1;

                if ((stop > 950) && (stop < 2600)) {
                    if (stop < 1000)
                        stop = 0;
                    else
                        stop -= 1000;

                    if (stop > 1023)
                        stop = 1023;


                    avbuf[avbuf_index] = stop;
                    avbuf_index = (avbuf_index + 1) & 0x07;

                    // Calculate the average
                    average = avbuf[0];
                    for(j=1; j < 8; j++)
                        average += avbuf[j];
                    average >>= 3;

                    if (state == STATE_ZEROING) {

                        if (is_low_zero(average, 0)) {
                            stability--;
                            if (stability == 0) {
                                state = STATE_UNIDIR;
                                zero = average;
                                mark_unidirectional();
                            }
                        }
                        else if (is_mid_zero(average, MID_POINT)) {
                            stability--;
                            if (stability == 0) {
                                state = STATE_BIDIR;
                                zero = average;
                                mark_bidirectional();
                            }
                        }
                        else
                            stability = ZERO_STABILITY;
                    }
                    else if (state == STATE_UNIDIR) {
                        // Unidirectional mode
                        if (is_low_zero(average, 0)) {
                            pwm_set(0);
                            led_set_mode(OFF);
                        }
                        else {
                            pwm_set(average >> 2);
                            led_set_mode(FAST);
                        }
                    }
                    else if (state == STATE_BIDIR) {
                        // Bidirectional mode
                        if (is_mid_zero(average, MID_POINT)) {
                            pwm_set(0);
                            led_set_mode(OFF);
                        }
                        else {
                            z = average - zero;

                            if (z > 0)
                                z >>= 1;
                            else
                                z = (z >> 1) | 0x8000;
                            pwm_set(z);

                            led_set_mode(FAST);
                        }
                    }
                }
            }

        }
        else
            if (inpsr == 0x0f) {
                rc_state = 1;
                T1CONbits.TMR1ON = 0;
                TMR1 = 0;
                T1CONbits.TMR1ON = 1;
            }
    }
}

// Global interrupt handler
interrupt void handler()
{
    led_interrupt_handler();
    INTCONbits.T0IF = 0;
}


