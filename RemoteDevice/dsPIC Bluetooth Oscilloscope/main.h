#include "p33fxxxx.h"
#include "ser.h"
#include "adc.h"
#include "timer.h"

#define MAX_LEVEL		240

#define DATA_START		(MAX_LEVEL + 1)
#define DATA_END		(MAX_LEVEL + 2)

#define REQ_DATA		0x00
#define ADJ_HORIZONTAL	0x01
#define ADJ_VERTICAL 	0x02
#define ADJ_POSITION	0x03
#define CHANNEL1		0x01
#define CHANNEL2		0x02

#define OFFSET1			510	//511
#define OFFSET2			495	//511

#define led_on()	PORTCbits.RC3 = 1
#define led_off()	PORTCbits.RC3 = 0

void osc_init(void);
void port_init(void);
void store_data(void);
void send_data(void);
unsigned char clamp_value(long value);

long ch1_gain, ch2_gain;
long ch1_position, ch2_position;



