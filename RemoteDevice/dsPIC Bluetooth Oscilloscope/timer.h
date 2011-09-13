#ifndef _TIMER_H_
#define _TIMER_H_

#define FCY				40000000	// 40MIPS
#define SAMPLES_PER_DIV 32

#define PERIOD_5us		(unsigned int)(FCY * 5E-6 / SAMPLES_PER_DIV )	// 6
#define PERIOD_10us		(unsigned int)(FCY * 10E-6 / SAMPLES_PER_DIV )	// 12
#define PERIOD_20us		(unsigned int)(FCY * 20E-6 / SAMPLES_PER_DIV )
#define PERIOD_50us		(unsigned int)(FCY * 50E-6 / SAMPLES_PER_DIV )
#define PERIOD_100us	(unsigned int)(FCY * 100E-6 / SAMPLES_PER_DIV )
#define PERIOD_200us	(unsigned int)(FCY * 200E-6 / SAMPLES_PER_DIV )
#define PERIOD_500us	(unsigned int)(FCY * 500E-6 / SAMPLES_PER_DIV )
#define PERIOD_1ms		(unsigned int)(FCY * 1E-3 / SAMPLES_PER_DIV )
#define PERIOD_2ms		(unsigned int)(FCY * 2E-3 / SAMPLES_PER_DIV )
#define PERIOD_5ms		(unsigned int)(FCY * 5E-3 / SAMPLES_PER_DIV )
#define PERIOD_10ms		(unsigned int)(FCY * 10E-3 / SAMPLES_PER_DIV )	// 12500
#define PERIOD_20ms		(unsigned int)(FCY * 20E-3 / SAMPLES_PER_DIV )
#define PERIOD_50ms		(unsigned int)(FCY * 50E-3 / SAMPLES_PER_DIV )	// 62500

extern volatile unsigned int sampling_period;
extern const unsigned int PERIODS[];

extern void timer1_init(void);
extern void set_sampling_period(unsigned int period);

#endif	// _TIMER_H_

