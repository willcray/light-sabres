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
	// 1 MHz / 8 = 125 kHz --> 260.4166667 ticks for 480 Hz
	// From MSP User Guide: The number of timer counts in the period is TACCR0+1
	TACCR0 = 259;

	// enable interrupt on capture-compare control register 0
	TACCTL0 |= CCIE;	// "All CCIFG flags request an interrupt
						// when their corresponding CCIE bit and
						// the GIE bit are set." - UserGuide

	// Set ADC to measure input channel 7, aka pin 1.7
	// Also set ADC to measure at 1/4 the normal rate
	ADC10CTL1 = INCH_7 + ADC10DIV_3;

	// Set ADC reference to VREF+, aka pin 1.4 on MSP430G2553
	// Also set sample and hold time to 64 x ADC10CLKs
	// Also set Reference generator to ON
	// Also set ADC10 to ON
	ADC10CTL0 = SREF_1 + ADC10SHT_3 + REF2_5V + REFON + ADC10ON;

	__delay_cycles(1000); // wait for ADC Ref to settle

	// Set ADC to enable conversion
	// Also set ADC to start converting
	ADC10CTL0 |= ENC + ADC10SC;
	calibrateValue = ADC10MEM;

	// calibrate indices of samples
	int i;
	for (i = 0; i < NUM_SAMPLES; i++) {
		samples[i] = calibrateValue;
	}
	average = calibrateValue;

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

	// this is run every 1/480Hz when Timer A hits comparison value

	// red LED on
	P1OUT |= BIT0;

	// sampling and conversion start
	ADC10CTL0 |= ENC + ADC10SC;

	// keep track of the value we're about to overwrite
	oldestReading = samples[index];

	// sample temperature sensor
	// store this sample in the oldest slot in the circular array 'samples'
	samples[index] = ADC10MEM;

	// calculate average of the samples in the sampling window
	filter();

	// update index "pointer" if it tries to roll off the end
	// of the circular array
	index = index + 1;
	if (index == NUM_SAMPLES)
		index = 0; // this is more efficient than modulus

	// red LED off
	P1OUT &= ~BIT0;
}
