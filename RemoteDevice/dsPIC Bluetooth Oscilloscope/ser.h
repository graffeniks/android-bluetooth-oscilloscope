#ifndef _SER_H_
#define _SER_H_

#define FCY 40000000					// = Fclk/2
#define BAUDRATE 115200					// bps
#define BRGVAL ((FCY/BAUDRATE)/4)-1		// for high-speed mode

/* Valid buffer size value are only power of 2 (ex: 2,4,..,64,128) */
#define SER_BUFFER_SIZE		64		
#define SER_FIFO_MASK 		(SER_BUFFER_SIZE-1)

extern void ser_init(void);
extern void ser_putch(unsigned char byte);
extern void ser_puts(unsigned char * s);
extern void ser_puthex(unsigned char v);
extern unsigned char ser_isrx(void);
extern unsigned char ser_getch(void);
extern void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void);
extern void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);

extern unsigned char txfifo[SER_BUFFER_SIZE];
extern volatile unsigned char txiptr, txoptr;
extern unsigned char rxfifo[SER_BUFFER_SIZE];
extern volatile unsigned char rxiptr, rxoptr;
extern unsigned char ser_tmp;

#endif // end _SER_H_
