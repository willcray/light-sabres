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

#define NUM_SAMPLES 8 // if you change the number of samples, you need to change the number of bit shifts you do to take the average on line 61
// change the value to log2(NUM_SAMPLES)

volatile int rowsToLight;
int n_x = 0; // index value
int j; // for loop variable
long v_in = 0; // value being read in from headphone jack
int sum = 0; // sum for the average calculation
long average =0; // the average value of the array
int samples[NUM_SAMPLES]; // instantiate array

// begin function definitions (we can move these to different file later)
void initPins(void);
void turnOnRows(void);
void ConfigureAdc(void);		// header for function
void filter(void);				// header for function

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

	/*
	// Set ADC to measure input channel 7, aka pin 1.7
	// Also set ADC to measure at 1/4 the normal rate
	ADC10CTL1 = INCH_7 + ADC10DIV_3;

	// Set ADC reference to VREF+, aka pin 1.4 on MSP430G2553
	// Also set sample and hold time to 64 x ADC10CLKs
	// Also set Reference generator to ON
	// Also set ADC10 to ON
	ADC10CTL0 = SREF_1 + ADC10SHT_3 + REF2_5V + REFON + ADC10ON;
	*/

	ConfigureAdc();					// call the configure function

	__delay_cycles(1000); // wait for ADC Ref to settle

/*
	// Set ADC to enable conversion
	// Also set ADC to start converting
	ADC10CTL0 |= ENC + ADC10SC;

	// set P1.6 as analog in
	P1SEL != BIT6;
	v_in = ADC10MEM;
  */

	// right before entering main loop, Global Interrupt Enable
	_BIS_SR(GIE);

	rowsToLight = 0;

	initPins();

	// begin main loop
	while (1) {
		rowsToLight = average; // LEDs lit 3 rows when no music playing but headphones plugged in.
		turnOnRows();
	}
	return 0;
}

void turnOnRows() {
	if (rowsToLight >= 0 && rowsToLight < 3) {
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
	} else if (rowsToLight == 3) {
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
	} else if (rowsToLight == 4) {
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
	} else if (rowsToLight == 5) {
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
	} else if (rowsToLight == 6) {
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
	} else if (rowsToLight == 7) {
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
	} else if (rowsToLight == 8) {
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
	} else if (rowsToLight == 9) {
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
	} else if (rowsToLight == 10) {
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
	} else if (rowsToLight == 11) {
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
	} else if (rowsToLight == 12) {
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
}

#pragma vector = TIMER0_A0_VECTOR         // Timer A interrupt service routine, add code here for IN LAB
__interrupt void TimerA0_routine(void) {
	ADC10CTL0 |= ENC + ADC10SC; // sampling and conversion start to store data in memory
	v_in = ADC10MEM; // extract data from the memory storage of the input voltage
	sum -= samples[n_x];    // get rid of current value
	samples[n_x] = v_in; // input v_in data here
	sum += samples[n_x];    // add in new value to sum
	n_x = (n_x + 1) % NUM_SAMPLES; // iterate through the array
	filter();         // call filter function
}

void filter() {
	_nop();             // do a no op
	average = sum >> 3; // change value being bit shifted by to log2(NUM_SAMPLES)
	// filter function optimized with bit shift function
}

void ConfigureAdc(void){
	ADC10CTL1 = ADC10DIV_3 + INCH_6;		// divide clock by one
	ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + REFON; //REF2_5V; // Vcc & Vss as reference, 64 clock cycles, turn on the ADC10 core, turn on the reference generator, make reference voltage 2.5 V
	ADC10AE0 |= ANALOG_IN_P1;						// ADC input enable P1.6
}
