////////////////////////////////////////
//
// Load driver PWM related functions
//
////////////////////////////////////////

// Description: The load PWM module controls the PWM signal
//              delivered to the electrical load. It uses
//              the controller's PWM peripheral (driven by timer 2)
//

// Initializes the PWM module
void pwm_init();

// Sets the PWM drive level. A value of zero is the minimal
// drive level (none) and a value of 255 is the maximal drive
// level. The sign of the argument determines the direction
void pwm_set(int value);
