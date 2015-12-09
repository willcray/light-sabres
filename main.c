#include <msp430.h> 
/*
 * ECE 3430 - FINAL PROJECT - LIGHT SABRES
 * Ben Browning, Steven Jenny, Will Cray
 */

// Define the different output pins corresponding to the different rows
// on the V sabres.

// port 1 pins
#define RED_LED_P1 BIT0 // P1.0
#define ROW_8_P1 BIT1 // P1.1
#define ROW_7_P1 BIT2 // P1.2
#define ROW_6_P1 BIT3 // P1.3
#define ROW_1_P1 BIT4 // P1.4
#define ROW_4_P1 BIT5 // P1.5
#define ANALOG_IN_P1 BIT6 // P1.6 analog voltage in for music
#define UNUSED_P1 BIT7 // P1.7

// begin using P2 pins
#define ROW_0_P2 BIT0 // P2.0
#define ROW_3_P2 BIT1 // P2.1
#define ROW_2_P2 BIT2 // P2.2
#define ROW_9_P2 BIT3 // P2.3
#define ROW_5_P2 BIT4 // P2.4
#define UNUSED_P2 BIT5 // P2.5

volatile int rowsToLight;

// begin function definitions (we can move these to different file later)
void initPins(void);
void turnOnRows(void);

int main(void) {

	// Stop watchdog timer
	WDTCTL = WDTPW | WDTHOLD;
	DCOCTL = CALDCO_16MHZ;  			// |Set clock speed to 16 MHz|
	BCSCTL1 = CALBC1_16MHZ;  			// |                         |

	// setup Timer A
	// SMCLK clock
	// divided by 8 (TASSEL_2)
	// in up mode (MC_1)
	// and start by clearing the timer
	TACTL = TASSEL_2 | ID_3 | MC_1 | TACLR;

	// setup value for comparison (timer in up mode counts until TACCR0 and then resets)
	// 16 MHz / 8 = 2 MHz --> 2000000 ticks per second
	// From MSP User Guide: The number of timer counts in the period is TACCR0+1
	TACCR0 = 65500;

	// enable interrupt on capture-compare control register 0
	TACCTL0 |= CCIE;	// "All CCIFG flags request an interrupt
						// when their corresponding CCIE bit and
						// the GIE bit are set." - UserGuide

	// right before entering main loop, Global Interrupt Enable
	_BIS_SR(GIE);

	rowsToLight = 0;

	initPins();

	// begin main loop
	while (1) {
		turnOnRows();
	}
	return 0;
}

void turnOnRows() {
	if (rowsToLight == 0) {
		P2OUT &= ~ROW_0_P2;
		P1OUT &= ~ROW_1_P1;
		P2OUT &= ~ROW_2_P2;
		P2OUT &= ~ROW_3_P2;
		P1OUT &= ~ROW_4_P1;
		P2OUT &= ~ROW_5_P2;
		P1OUT &= ~ROW_6_P1;
		P1OUT &= ~ROW_7_P1;
		P1OUT &= ~ROW_8_P1;
		P2OUT &= ~ROW_9_P2;
	} else if (rowsToLight == 1) {
		P2OUT |= ROW_0_P2;
		P1OUT &= ~ROW_1_P1;
		P2OUT &= ~ROW_2_P2;
		P2OUT &= ~ROW_3_P2;
		P1OUT &= ~ROW_4_P1;
		P2OUT &= ~ROW_5_P2;
		P1OUT &= ~ROW_6_P1;
		P1OUT &= ~ROW_7_P1;
		P1OUT &= ~ROW_8_P1;
		P2OUT &= ~ROW_9_P2;
	} else if (rowsToLight == 2) {
		P2OUT |= ROW_0_P2;
		P1OUT |= ROW_1_P1;
		P2OUT &= ~ROW_2_P2;
		P2OUT &= ~ROW_3_P2;
		P1OUT &= ~ROW_4_P1;
		P2OUT &= ~ROW_5_P2;
		P1OUT &= ~ROW_6_P1;
		P1OUT &= ~ROW_7_P1;
		P1OUT &= ~ROW_8_P1;
		P2OUT &= ~ROW_9_P2;
	} else if (rowsToLight == 3) {
		P2OUT |= ROW_0_P2;
		P1OUT |= ROW_1_P1;
		P2OUT |= ROW_2_P2;
		P2OUT &= ~ROW_3_P2;
		P1OUT &= ~ROW_4_P1;
		P2OUT &= ~ROW_5_P2;
		P1OUT &= ~ROW_6_P1;
		P1OUT &= ~ROW_7_P1;
		P1OUT &= ~ROW_8_P1;
		P2OUT &= ~ROW_9_P2;
	} else if (rowsToLight == 4) {
		P2OUT |= ROW_0_P2;
		P1OUT |= ROW_1_P1;
		P2OUT |= ROW_2_P2;
		P2OUT |= ROW_3_P2;
		P1OUT &= ~ROW_4_P1;
		P2OUT &= ~ROW_5_P2;
		P1OUT &= ~ROW_6_P1;
		P1OUT &= ~ROW_7_P1;
		P1OUT &= ~ROW_8_P1;
		P2OUT &= ~ROW_9_P2;
	} else if (rowsToLight == 5) {
		P2OUT |= ROW_0_P2;
		P1OUT |= ROW_1_P1;
		P2OUT |= ROW_2_P2;
		P2OUT |= ROW_3_P2;
		P1OUT |= ROW_4_P1;
		P2OUT &= ~ROW_5_P2;
		P1OUT &= ~ROW_6_P1;
		P1OUT &= ~ROW_7_P1;
		P1OUT &= ~ROW_8_P1;
		P2OUT &= ~ROW_9_P2;
	} else if (rowsToLight == 6) {
		P2OUT |= ROW_0_P2;
		P1OUT |= ROW_1_P1;
		P2OUT |= ROW_2_P2;
		P2OUT |= ROW_3_P2;
		P1OUT |= ROW_4_P1;
		P2OUT |= ROW_5_P2;
		P1OUT &= ~ROW_6_P1;
		P1OUT &= ~ROW_7_P1;
		P1OUT &= ~ROW_8_P1;
		P2OUT &= ~ROW_9_P2;
	} else if (rowsToLight == 7) {
		P2OUT |= ROW_0_P2;
		P1OUT |= ROW_1_P1;
		P2OUT |= ROW_2_P2;
		P2OUT |= ROW_3_P2;
		P1OUT |= ROW_4_P1;
		P2OUT |= ROW_5_P2;
		P1OUT |= ROW_6_P1;
		P1OUT &= ~ROW_7_P1;
		P1OUT &= ~ROW_8_P1;
		P2OUT &= ~ROW_9_P2;
	} else if (rowsToLight == 8) {
		P2OUT |= ROW_0_P2;
		P1OUT |= ROW_1_P1;
		P2OUT |= ROW_2_P2;
		P2OUT |= ROW_3_P2;
		P1OUT |= ROW_4_P1;
		P2OUT |= ROW_5_P2;
		P1OUT |= ROW_6_P1;
		P1OUT |= ROW_7_P1;
		P1OUT &= ~ROW_8_P1;
		P2OUT &= ~ROW_9_P2;
	} else if (rowsToLight == 9) {
		P2OUT |= ROW_0_P2;
		P1OUT |= ROW_1_P1;
		P2OUT |= ROW_2_P2;
		P2OUT |= ROW_3_P2;
		P1OUT |= ROW_4_P1;
		P2OUT |= ROW_5_P2;
		P1OUT |= ROW_6_P1;
		P1OUT |= ROW_7_P1;
		P1OUT |= ROW_8_P1;
		P2OUT &= ~ROW_9_P2;
	} else if (rowsToLight == 10) {
		P2OUT |= ROW_0_P2;
		P1OUT |= ROW_1_P1;
		P2OUT |= ROW_2_P2;
		P2OUT |= ROW_3_P2;
		P1OUT |= ROW_4_P1;
		P2OUT |= ROW_5_P2;
		P1OUT |= ROW_6_P1;
		P1OUT |= ROW_7_P1;
		P1OUT |= ROW_8_P1;
		P2OUT |= ROW_9_P2;
	} else {
		// some error...?
		P2OUT &= ~ROW_0_P2;
		P1OUT |= ROW_1_P1;
		P2OUT &= ~ROW_2_P2;
		P2OUT |= ROW_3_P2;
		P1OUT &= ~ROW_4_P1;
		P2OUT |= ROW_5_P2;
		P1OUT &= ~ROW_6_P1;
		P1OUT |= ROW_7_P1;
		P1OUT &= ~ROW_8_P1;
		P2OUT |= ROW_9_P2;
	}
}

void initPins(void) {

	// set P1.6 as analog input for the music
	P1DIR &= ~ANALOG_IN_P1;

	// set all LED rows to low at the beginning
	P2OUT &= ~ROW_0_P2;
	P1OUT &= ~ROW_1_P1;
	P2OUT &= ~ROW_2_P2;
	P2OUT &= ~ROW_3_P2;
	P1OUT &= ~ROW_4_P1;
	P2OUT &= ~ROW_5_P2;
	P1OUT &= ~ROW_6_P1;
	P1OUT &= ~ROW_7_P1;
	P1OUT &= ~ROW_8_P1;
	P2OUT &= ~ROW_9_P2;

	// set all pins to output direction
	P2DIR |= ROW_0_P2;
	P1DIR |= ROW_1_P1;
	P2DIR |= ROW_2_P2;
	P2DIR |= ROW_3_P2;
	P1DIR |= ROW_4_P1;
	P2DIR |= ROW_5_P2;
	P1DIR |= ROW_6_P1;
	P1DIR |= ROW_7_P1;
	P1DIR |= ROW_8_P1;
	P2DIR |= ROW_9_P2;

	// Enable Analog input for ADC on P1.6
	ADC10AE0 |= ANALOG_IN_P1;
}

// Timer A interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TimerA0_routine(void) {

	// this is run every second when Timer A hits comparison value

	// red LED on
	P1OUT |= BIT0;

	rowsToLight = rowsToLight + 1;
	if (rowsToLight > 10)
		rowsToLight = 0; // this is more efficient than modulus

	// red LED off
	P1OUT &= ~BIT0;
}
