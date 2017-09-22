
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include <stdbool.h>
#include <string.h>


#include "lcd.h"
#include "rotary.h"
#include "timer.h"
#include "usart.h"

#ifndef F_CPU
//define cpu clock speed if not defined
#define F_CPU 8000000
#endif

void updateLCD(uint16_t seconds)
	{

		uint8_t mins = 0;
		uint8_t minStrSize = 0;
		uint8_t secStrSize = 0;
		char minStr[12];
		char secStr[12];
		//char minLen[12];
		//char secLen[12];



		lcd_gotoxy(0,0);
	    lcd_puts(utoa(seconds,minStr,10)); //output the raw calling value

	    mins  = seconds / 60; // calculate the minutes
		seconds = seconds % 60; //calculate the remaining seconds

		(utoa(mins,minStr,10)); //convert minutes into string
		minStrSize = strlen(minStr); //length of the string
		(utoa(seconds,secStr,10)); //convert seconds into string
		secStrSize = strlen(secStr); //length of the string
		if (secStrSize==1)
			strcat("0",secStr);




		//lcd_gotoxy(0,1);
		//lcd_puts(itoa(minStrSize,minLen,10)); //output the length of the string


		lcd_gotoxy(13-minStrSize,1); //position depends on #digits
		lcd_puts(minStr);



		lcd_gotoxy(13,1); //go to 13th position on second line
		lcd_puts(":"); //output the separator
		lcd_gotoxy(14,1); //go to 13th position on second line
		lcd_puts(secStr); //output the seconds
	}

void updateSerial(uint16_t seconds)
	{
	uint16_t mins = 0;
	uint16_t secs = 0;
	char tmpmins[100];
	char tmpsecs[100];
		if (seconds < 0)
			seconds = 0;
	mins  = seconds / 60;
	secs = seconds % 60;

	(itoa(mins,tmpmins,10));
	(itoa(secs,tmpsecs,10));
	USART_putstring("\"");
	USART_putstring(tmpmins);  // Pass the string to the USART_putstring function and sends it over the serial
	USART_putstring(":");
	USART_putstring(tmpsecs);
	USART_putstring("\"");
	USART_putstring("\n");

	}

int main(void) {
	//initialize peripherals
	DDRB = 0x01; //Set as output for LED
	lcd_init(LCD_DISP_ON); // Initialize the LCD
	USART_init();
	lcd_clrscr();
	lcd_gotoxy(0,0);
	lcd_puts("Turn=set time");
	lcd_gotoxy(0,1);
	//lcd_puts("push=start");
	RotaryInit(); //init rotary functions
	Timer0_Init();
	Timer0_Start();
	sei();
	USART_init();   // Initialize USART
	uint16_t secs = 0; //counter for turns of rotary encoder
	//char * tmpstr[12];
	secs = eeprom_read_word((uint16_t *) 0);
	//if ( secs == 65535 )
		secs = 1601;
	bool isTimeSet = false;
	//start loop
	while(!isTimeSet) //Setting time
	{
		lcd_gotoxy(0,1); //go to second line
		switch (RotaryGetStatus())
		{
		case 1:
			//lcd_puts("RIGHT ");
			secs++; //increase seconds counter
			break;
		case 2:
			//lcd_puts("LEFT  ");
			secs--;
			break;
		case 3:
			//lcd_puts("BUTTON");
			eeprom_update_word((uint16_t *) 0, secs);
			//isTimeSet = true; //stops the while loop
			break;
		}
		RotaryResetStatus(); //reset
		//lcd_gotoxy(11,1); //go to 11th position on second line
		//lcd_puts(utoa(secs,tmpstr,10)); //output the counter
		updateSerial(secs);
		updateLCD(secs);

	
	} //while
	while(1)
	{
		lcd_gotoxy(0,1); //go to second line
		lcd_puts("Counting down   ");
	}
}
