
#include "p33fxxxx.h"
#include "timer.h"

volatile unsigned int sampling_period;
const unsigned int PERIODS[]={
	PERIOD_5us,
	PERIOD_10us,
	PERIOD_20us,
	PERIOD_50us,
	PERIOD_100us,
	PERIOD_200us,
	PERIOD_500us,
	PERIOD_1ms,
	PERIOD_2ms,
	PERIOD_5ms,
	PERIOD_10ms,
	PERIOD_20ms,
	PERIOD_50ms
};

void timer1_init(void)
{
	T1CONbits.TON = 0;	 	// Stops 16-bit Timer1
	T1CONbits.TCKPS = 0b00;	// Timer1 Input Clock Prescale Select bits
	T1CONbits.TCS = 0;		// Internal clock (FCY)
	T1CONbits.TGATE = 0;	// 	Gated time accumulation disabled

	set_sampling_period(PERIOD_200us);

	//T1CONbits.TON = 1;		// Starts 16-bit Timer1
}

void set_sampling_period(unsigned int period)
{
#if 0
	PR1 = period;
#else	// adc speed limitation
	if( (period==PERIOD_5us) || (period==PERIOD_10us) )
		PR1 = PERIOD_20us;
	else
		PR1 = period;
#endif
	sampling_period = period;
}
