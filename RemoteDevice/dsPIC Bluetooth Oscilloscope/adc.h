#ifndef _ADC_H_
#define _ADC_H_

#define MAX_SAMPLES	640	// 320 * 2 channels

#define kSCALE	6.2
#define SCALE_10mV		(long)(kSCALE / 0.01)
#define SCALE_20mV		(long)(kSCALE / 0.02)
#define SCALE_50mV		(long)(kSCALE / 0.05)
#define SCALE_100mV		(long)(kSCALE / 0.1)
#define SCALE_200mV		(long)(kSCALE / 0.2)
#define SCALE_500mV		(long)(kSCALE / 0.5)
#define SCALE_1V		(long)(kSCALE / 1.0)
#define SCALE_2V		(long)(kSCALE / 2.0)
#define SCALE_GND		(long)(0)


extern volatile unsigned int adc_value_1;
extern volatile unsigned int adc_value_2;
extern volatile unsigned int raw_data[MAX_SAMPLES];
extern const long VOLT_PER_DIV[];

extern void adc_init(void);
extern void aux_pll_init(void);
extern void store_raw_data(void);
extern unsigned int get_total_samples(void);

#endif // end _ADC_H_
