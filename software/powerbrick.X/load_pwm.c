//////////////////////////////////////////////////////
//
// Load driver PWM related functions - implementation
//
//////////////////////////////////////////////////////

#include <htc.h>
#include "hardware.h"
#include "load_pwm.h"

// According to the PIC12F615 datasheet, PWM steering from PWM_A to PWM_B
// is possible. However, there is no register controlling the steering operation,
// suggesting that it's not actually possible.
// To overcome this problem, a nasty trick is used. The PWM peripheral is configured
// to single bridge mode with complementaty output. This way, the waveforms of
// PWM_A and PWM_B are identical. To steer the signal to one of the pins, the
// tristate register of the respective pin is put in output mode, while the
// second is put in tri-state mode, which is pulled low by the extenal pull-down
// resistor.

unsigned char direction;

#define DIR_FORWARD     0
#define DIR_REVERSE     1
#define DIR_UNDEFINED   99

void pwm_init()
{
    // PWM Peripheral Setup
    CCP1CONbits.CCP1M = 13; // PWM Mode, P1A active-high; P1B active-low
    CCP1CONbits.P1M = 1;    // Select half-bridge mode
    PR2 = 0x65; // Period set
    CCPR1L = 0x00; // 0% duty initially
    T2CONbits.TMR2ON = 1; // Turn on timer 2

    // Initially set the direction to an invalid value
    direction = DIR_UNDEFINED;
}

void pwm_set(int value)
{
    if (value >= 0) {
        // Forward direction
        if (direction != DIR_FORWARD) {
            PWM_A_TRIS = 0;
            PWM_B_TRIS = 1;
        }
        CCPR1L = (value & 0xff);
    }
    else {
        // Reverse direction
        if (direction != DIR_REVERSE) {
            PWM_A_TRIS = 1;
            PWM_B_TRIS = 0;
        }
        CCPR1L = ((-value) & 0xff);
    }
}

