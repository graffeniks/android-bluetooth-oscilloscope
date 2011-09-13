#define _SER_C_
#include "p33fxxxx.h"
#include "ser.h"

unsigned char txfifo[SER_BUFFER_SIZE];
volatile unsigned char txiptr, txoptr;
unsigned char rxfifo[SER_BUFFER_SIZE];
volatile unsigned char rxiptr, rxoptr;
unsigned char ser_tmp;

void ser_init(void)
{
	//RPINR18bits.U1RXR = 28;		//RX -> pin 20 (RP28)	
	//RPOR13bits.RP27R = 0b00011;	//TX -> pin 19 (RP27)

	U1MODEbits.STSEL = 0;	// 1-stop bit
	U1MODEbits.PDSEL = 0;	// No Parity, 8-data bits
	U1MODEbits.ABAUD = 0;	// Auto-Baud Disabled
	U1MODEbits.BRGH = 1;	//High-Speed mode
	U1BRG = BRGVAL;			// BAUD Rate Setting
	U1STAbits.UTXISEL0 = 0; // Interrupt after one Tx character is transmitted
	U1STAbits.UTXISEL1 = 0;
	U1STAbits.URXISEL = 0b00; // Interrupt after one RX character is received;
	IEC0bits.U1RXIE = 1;		//Enable RX interrupt
	U1MODEbits.UARTEN = 1; // Enable UART
	U1STAbits.UTXEN = 1; // Enable UART Tx

	//set ring pointers to empty (zero receive/transmit)
	rxiptr=rxoptr=txiptr=txoptr=0;	
}
void ser_putch(unsigned char byte)
{
	// wait until buffer has an empty slot.
	//while (((txiptr+1) & SER_FIFO_MASK)==txoptr)	
	while (U1STAbits.UTXBF)
		continue;
	txfifo[txiptr] = byte;			//place character in buffer

	//increase ring input pointer and set it to zer0 if 
	//it has rolled-it over.
	txiptr=(txiptr+1) & SER_FIFO_MASK;		
	
	IEC0bits.U1TXIE = 1;		//enable interrupt driven 
								//serial transmission
}
void ser_puts(unsigned char * s)
{
	while(*s)					//while pointer s is not at end of string
		ser_putch(*s++);		//	send the current character, 
								//	then increment pointer
}
void ser_puthex(unsigned char v)
{
	unsigned char c;			//define temp variable

	c = v >> 4;					//get the high nibble and place it in c
	if (c>9) {					//if more than 9
		ser_putch('A'-10+c);	//	send the difference from 10 + 'A'
	} else {					//else
		ser_putch('0'+c);		//	send '0' + the high nibble
	}

	c = v & 0x0F;				//get the lower nibble
	if (c>9) {					//process the character 
		ser_putch('A'-10+c);	//and send it same as above
	} else {					//
		ser_putch('0'+c);		//
	}
}
unsigned char ser_isrx(void)
{
	if(U1STAbits.OERR)	//error in reception?
	{
		U1STAbits.OERR = 0;	//must clear the overrun error to keep uart receiving
		return 0;			//	return no characters in buffer
	}
	return (rxiptr!=rxoptr);	//checks buffer if char is present
}
unsigned char ser_getch(void)
{
	unsigned char c;			//define

	while (ser_isrx()==0)		//wait until a character is present
		continue;				//

	c=rxfifo[rxoptr];			//get oldest character received
	++rxoptr;					//move the pointer to discard buffer
	rxoptr &= SER_FIFO_MASK;	//if the pointer is at end, roll-it over.
	return c;					//return it
}
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void)
{
	U1TXREG = txfifo[txoptr];
	++txoptr;
	txoptr &= SER_FIFO_MASK;
	if (txoptr==txiptr)		IEC0bits.U1TXIE = 0;
}
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
	rxfifo[rxiptr]=U1RXREG;
	ser_tmp=(rxiptr+1) & SER_FIFO_MASK;
	if (ser_tmp!=rxoptr)	rxiptr=ser_tmp;
	IFS0bits.U1RXIF = 0;
}
