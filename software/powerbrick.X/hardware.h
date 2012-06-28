///////////////////////////////
//
// Hardware related definitions
//
///////////////////////////////

// The LED is connected to GP1
#define LED_GPIO    GPIObits.GP1
#define LED_TRIS    TRISIO1

// A PWM driver is connected to GP0 and GP2
//   Only the TRIS registers are defined, since they are
//   driven directly by the PWM hardware
#define PWM_A_TRIS  TRISIO0
#define PWM_B_TRIS  TRISIO2

// Receiver PWM Input
#define RX_GPIO     GPIObits.GP4
#define RX_TRIS     TRISIO4
#define RX_ANSEL    ANSELbits.AN3

