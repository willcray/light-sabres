#include <msp430.h> 
/*
 * ECE 3430 - FINAL PROJECT - LIGHT SABRES
 * Ben Browning, Steven Jenny, Will Cray
 */

// define the different output pins corresponding to the different rows
// on the V sabres
#define ROW_1 BIT0
#define ROW_2 BIT1
#define ROW_3 BIT2
#define ROW_4 BIT3
#define ROW_5 BIT4
#define ROW_6 BIT5
#define ROW_7 BIT6
// begin using P2 pins
#define ROW_8 BIT0
#define ROW_9 BIT1
#define ROW_10 BIT2
// use P1.6 as the analog voltage in for music
#define V_IN BIT6


// begin function definitions (we can move these to different file later)
void initPins(void);

int main(void) {

	// Stop watchdog timer
	WDTCTL = WDTPW | WDTHOLD;

	// begin main loop
	while (1) {

	}
	return 0;
}

void initPins(void) {

	// set P1.6 as analog input for the music
	P1DIR &= ~V_IN;

	// set all LED rows to low at the beginning
	P1OUT &= ~ROW_1;
	P1OUT &= ~ROW_2;
	P1OUT &= ~ROW_3;
	P1OUT &= ~ROW_4;
	P1OUT &= ~ROW_5;
	P1OUT &= ~ROW_6;
	P1OUT &= ~ROW_7;
	// begin using P2 pins
	P2OUT &= ~ROW_8;
	P2OUT &= ~ROW_9;
	P2OUT &= ~ROW_10;
	// set all pins to output direction
	P1DIR |= ROW_0;
	P1DIR |= ROW_1;
	P1DIR |= ROW_2;
	P1DIR |= ROW_3;
	P1DIR |= ROW_4;
	P1DIR |= ROW_5;
	// begin using P2 pins
	P2DIR |= ROW_6;
	P2DIR |= ROW_7;
	P2DIR |= ROW_8;
	P2DIR |= ROW_9;
	P2DIR |= ROW_10;
}
