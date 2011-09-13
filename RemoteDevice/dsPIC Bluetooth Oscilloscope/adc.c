
#include "p33fxxxx.h"
#include "adc.h"
#include "timer.h"

volatile unsigned int adc_value_1;
volatile unsigned int adc_value_2;
volatile unsigned int raw_data[MAX_SAMPLES];
volatile unsigned int samples_count;

const long VOLT_PER_DIV[]={
	SCALE_10mV,
	SCALE_20mV,
	SCALE_50mV,
	SCALE_100mV,
	SCALE_200mV,
	SCALE_500mV,
	SCALE_1V,
	SCALE_2V,
	SCALE_GND
};

void adc_init(void)
{
	ADCONbits.ADON = 0;		// Temporarily Turn off ADC to allow for initialization settings

	aux_pll_init();
	ADCONbits.FORM = 0;			// Output in Integer Format
	ADCONbits.EIE = 1;			// Enable Early Interrupt (7TAD)
	//ADCONbits.ORDER = 0;		// Normal Order of conversion
	//ADCONbits.SEQSAMP = 0;	// Simultaneous sampling
	ADCONbits.ASYNCSAMP = 1;	// Asynchronous sampling
	ADCONbits.SLOWCLK = 0;		// High Frequency Clock input (use aux pll)
	ADCONbits.ADCS = 0;		//5; // Clock divider selection
	ADCPC1bits.TRGSRC2 = 0b01100;	// Timer1 period match (trigger source)
	ADPCFGbits.PCFG4 = 0;		// AN4 is configured as analog input
	ADPCFGbits.PCFG5 = 0;		// AN5 is configured as analog input
	IPC28bits.ADCP2IP = 0x01;	// Set ADC Pair 2 Interrupt Priority (Level 1)
	IFS7bits.ADCP2IF = 0;		// Clear ADC Pair 2 Interrupt Flag
	//IEC7bits.ADCP2IE = 1;		// Enable ADC Pair 2 Interrupt

	ADCONbits.ADON = 1; // Enable ADC module

}

/* ADC Pair 2 ISR*/
void __attribute__((interrupt, no_auto_psv)) _ADCP2Interrupt (void)
{
	raw_data[samples_count++] = ADCBUF4; // Read AN4 conversion result;
	raw_data[samples_count++] = ADCBUF5; // Read AN5 conversion result;
	IFS7bits.ADCP2IF = 0; // Clear ADC Pair 2 Interrupt Flag
}

void store_raw_data(void)
{
	unsigned int total_samples;

	total_samples = get_total_samples();
	samples_count = 0;		// reset counter

	//IEC0bits.U1RXIE = 0;	// disable RX interrupt
	IEC7bits.ADCP2IE = 1;	// enable ADC Pair 2 Interrupt
	TMR1 = 0x0000;			// reset timer 1
	T1CONbits.TON = 1;		// Starts 16-bit Timer1

	while(samples_count<total_samples);	// wait until samples are completed

	IEC7bits.ADCP2IE = 0;	// disable ADC Pair 2 Interrupt
	T1CONbits.TON = 0;	 	// Stops 16-bit Timer1
	//IEC0bits.U1RXIE = 1;	// Enable RX interrupt
}

unsigned int get_total_samples(void)
{
	if(sampling_period==PERIOD_5us)
		return (MAX_SAMPLES/4);
	else if(sampling_period==PERIOD_10us)
		return (MAX_SAMPLES/2);
	else
		return (MAX_SAMPLES);
}	

void aux_pll_init(void)
{
	ACLKCONbits.FRCSEL = 1;		// Internal FRC is clock source for auxiliary PLL
	ACLKCONbits.ENAPLL = 1;		// APLL is enabled
	ACLKCONbits.SELACLK = 1;	// Auxiliary PLL provides the source clock for the clock divider
	ACLKCONbits.APSTSCLR = 7;	// Auxiliary Clock Output Divider is Divide by 1
	while(ACLKCONbits.APLLCK != 1){};	// Wait for Auxiliary PLL to Lock
}
