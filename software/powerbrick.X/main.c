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

/*
 * 
 */
void main() {
    int i, j;

    // Initialization
    led_init();
    pwm_init();

    // Enable interrupts
    INTCONbits.GIE = 1;


    PWM_A_TRIS = 0;

    led_set_mode(ON);
    pwm_set(0);
    for(i=0; i < 900; i++) for(j=0; j < 100; j++);
    pwm_set(30);
    led_set_mode(OFF);
    for(i=0; i < 900; i++) for(j=0; j < 100; j++);
    pwm_set(-30);
    led_set_mode(FAST);
    for(i=0; i < 900; i++) for(j=0; j < 100; j++);
    pwm_set(200);
     led_set_mode(SLOW);

    while(1);
}

// Global interrupt handler
interrupt void handler()
{
    led_interrupt_handler();
    INTCONbits.T0IF = 0;
}


