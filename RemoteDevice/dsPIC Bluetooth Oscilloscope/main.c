/************************************
	Bluetooth Oscilloscope
	yus - projectproto.blogspot.com
	September 2010
*************************************/

#include "main.h"

_FOSCSEL(FNOSC_FRC);	// Select Internal FRC at POR
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF);	// Enable Clock Switching and Configure
_FICD(ICS_PGD1 & JTAGEN_OFF);

int main(void)
{
	unsigned char c;

	osc_init();
	port_init();
	ser_init();
	adc_init();
	timer1_init();

	//initial values;
	ch1_position = 120;
	ch2_position = 120;
	ch1_gain = VOLT_PER_DIV[4];
	ch2_gain = VOLT_PER_DIV[5];

	while(1)
	{
		if(ser_isrx()){
			led_off();
			c = ser_getch();		//get 1 character from receive buffer
			switch(c){
				case REQ_DATA:
					store_raw_data();
					ser_putch(DATA_START);
					send_data();
					ser_putch(DATA_END);
					break;
				case ADJ_HORIZONTAL:
					c = ser_getch();
					if( (c<13) ){
						set_sampling_period( PERIODS[c] );	// refer to "timer.h"
					}
					break;
				case ADJ_VERTICAL:
					c = ser_getch();
					if(c==CHANNEL1)			ch1_gain = VOLT_PER_DIV[ser_getch()];	// refer to "adc.h"
					else if(c==CHANNEL2)	ch2_gain = VOLT_PER_DIV[ser_getch()];
					break;
				case ADJ_POSITION:
					c = ser_getch();
					if(c==CHANNEL1)			ch1_position = (long)ser_getch() * 6;
					else if(c==CHANNEL2)	ch2_position = (long)ser_getch() * 6;
					break;
				default:
					break;
			} // switch(command)
			led_on();			
		}//if-ser_isrx()

	}//while-true
}

void osc_init(void)
{
	// Configure PLL prescaler, PLL postscaler, PLL divisor
	PLLFBD = 41; // M = 43
	CLKDIVbits.PLLPOST=0; // N1 = 2
	CLKDIVbits.PLLPRE=0; // N2 = 2
	// Initiate Clock Switch to Internal FRC with PLL (NOSC = 0b001)
	__builtin_write_OSCCONH(0x01);
	__builtin_write_OSCCONL(0x01);
	// Wait for Clock switch to occur
	while (OSCCONbits.COSC != 0b001);
	// Wait for PLL to lock
	while(OSCCONbits.LOCK!=1) {};
}

void port_init(void)
{
	// LED Pin Configuration:I/O Port RC3; pin 5
   	PORTCbits.RC3 = 0;				// Configure as Output
   	TRISCbits.TRISC3 = 0;			// Configure as Output
	LATCbits.LATC3 = 1;				// Initialize to zero

	RPINR18bits.U1RXR = 28;		//RX -> pin 20 (RP28)	
	RPOR13bits.RP27R = 0b00011;	//TX -> pin 19 (RP27)

	ADPCFGbits.PCFG4 = 0; // AN4 is configured as analog input
	ADPCFGbits.PCFG5 = 0; // AN5 is configured as analog input
}

void send_data(void)
{
	#define bits	10
	unsigned int i, total;
	unsigned int repeat, n;
	unsigned char plot_data1, plot_data2;
	signed long temp;
	total = get_total_samples();
	repeat = MAX_SAMPLES / total;	
	
	i=0;
	while( i<total )
	{
		/*** CHANNEL 1 ***/
		// refer to "adc.xmcd" (mathcad) for the computation
		temp = (ch1_position<<bits) / (long)MAX_LEVEL;
		temp = (long)OFFSET1 + temp - (long)raw_data[i++];
		temp = temp * (long)MAX_LEVEL * ch1_gain;
		temp = (temp>>bits) + (ch1_position<<2) - (ch1_position*ch1_gain);
		temp = temp>>2;
		plot_data1 = clamp_value( temp );

		/*** CHANNEL 2 ***/
		temp = (ch2_position<<bits) / (long)MAX_LEVEL;
		temp = (long)OFFSET2 + temp - (long)raw_data[i++];
		temp = temp * (long)MAX_LEVEL * ch2_gain;
		temp = (temp>>bits) + (ch2_position<<2) - (ch2_position*ch2_gain);
		temp = temp>>2;
		plot_data2 = clamp_value( temp );

		n=repeat;
		while(n--){
			ser_putch( plot_data1 );
			ser_putch( plot_data2 );
		}

	}
}

unsigned char clamp_value(long value)
{
	if(value<0)
		return 0;
	else if (value>(long)MAX_LEVEL)
		return ((unsigned char) MAX_LEVEL);
	else
		return (unsigned char) value;
}
