//////////////////////////////////////////////////////
//
// Load driver PWM related functions - implementation
//
//////////////////////////////////////////////////////

#include <htc.h>
#include "hardware.h"
#include "load_pwm.h"

void pwm_init()
{
    // PWM Peripheral Setup
    CCP1CONbits.P1M = 0;
    CCP1CONbits.CCP1M = 12; // PWM Mode, Active High
    PR2 = 0x65; // Period set
    CCPR1L = 0x00; // 0% duty initially
    T2CONbits.TMR2ON = 1; // Turn on timer 2
}

void pwm_set(int value)
{
    CCPR1L = (value & 0xff);
}

