/*
 * uart.h
 *
 * UART example for ATMega328P clocked at 8 MHz
 *
 * TODO :-
 */


#include <avr/io.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)


//Declaration of our functions
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);

