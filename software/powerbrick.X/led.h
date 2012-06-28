///////////////////////////////
//
// LED Related Functions
//
///////////////////////////////

// Description: The LED module provides several blinking modes.
//              It uses TIMER0 to work in background
//

// LED Blinking Modes
typedef enum {
    SLOW, FAST, OFF, ON, SINGLE
} led_mode_type;

// Initialize the LED related functions
void led_init();

// Sets the LED blinking mode to the one specified
void led_set_mode(led_mode_type newmode);

// Interrupt handler - must be called when TIMER0 interrupt occurs
void led_interrupt_handler();
