
	
#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "leds.h"
#include "klaw.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

volatile uint8_t S2_press=0;	// "1" - klawisz zostal wcisniety "0" - klawisz "skonsumowany"
volatile uint8_t S3_press=0;
volatile uint8_t S4_press=0;

char display[] ={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};

char *morse_code[] = {
    ".-  ",  // A
    "-...",  // B
    "-.-.",  // C
    "-.. ",  // D
    ".   ",  // E
    "..-.",  // F
    "--. ",  // G
    "....",  // H
    "..  ",  // I  
    ".---",  // J
    "-.- ",  // K
    ".-..",  // L
    "--  ",  // M
    "-.  ",  // N
    "--- ",  // O
    ".--.",  // P
    "--.-",  // Q
    ".-. ",  // R
    "... ",  // S
    "-   ",  // T
    "..- ",  // U
    "...-",  // V
    ".-- ",  // W
    "-..-",  // X
    "-.--",  // Y
    "--.."   // Z
		
};

char *letters[] = {
		" = A",
		" = B", 
		" = C",
		" = D",  
		" = E", 
		" = F",
		" = G",
		" = H",
		" = I",
		" = J",
		" = K",
		" = L",
		" = M",
		" = N",
		" = O",
		" = P",
		" = Q",
		" = R",
		" = S",
		" = T",
		" = U",
		" = V",
		" = W",
		" = X",
		" = Y",
		" = Z"
};



void PORTA_IRQHandler(void)	// Podprogram obslugi przerwania od klawiszy S2, S3 i S4
{
	uint32_t buf;
	buf=PORTA->ISFR & (S1_MASK | S2_MASK | S3_MASK | S4_MASK);

	switch(buf)
	{
		case S2_MASK:	DELAY(10)
									if(!(PTA->PDIR&S2_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S2_MASK))	// Minimalizacja drgan zestyków (c.d.)
										{
											if(!S2_press)
											{
												S2_press=1;
											}
										}
									}
									break;
		case S3_MASK:	DELAY(10)
									if(!(PTA->PDIR&S3_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S3_MASK))	// Minimalizacja drgan zestyków (c.d.)
										{
											if(!S3_press)
											{
												S3_press=1;
											}
										}
									}
									break;
		case S4_MASK:	DELAY(10)
									if(!(PTA->PDIR&S4_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S4_MASK))	// Minimalizacja drgan zestyków (c.d.)
										{
											if(!S4_press)
											{
												S4_press=1;
											}
										}
									}
									break;
		default:			break;
	}	
	PORTA->ISFR |=  S1_MASK | S2_MASK | S3_MASK | S4_MASK;	// Kasowanie wszystkich bitów ISF
	NVIC_ClearPendingIRQ(PORTA_IRQn);
}

int main (void) 
{
	char morse_code_string[5] = {0x20,0x20,0x20,0x20, '\0'};
	Klaw_Init();								// Inicjalizacja klawiatury
	Klaw_S2_4_Int();						// Klawisze S2, S3 i S4 zglaszaja przerwanie
	LED_Init();									// Inicjalizacja diod LED
	LCD1602_Init();		 					// Inicjalizacja LCD
	LCD1602_Backlight(TRUE);  	// Wlaczenie podswietlenia
	
	LCD1602_ClearAll();					// Wyczysc ekran
	LCD1602_Print("Alfabet Morse'a");		// Wyswietl powitanie (ciag tekstowy)
	LCD1602_SetCursor(0,1);			// Ustaw kursor na poczatku drugiej linii
	LCD1602_Print("converter.");	// Wyswietl powitanie c.d.
	
	while(PTA->PDIR&S1_MASK);  	// Czy klawisz S1 wcisniety? (oczekiwanie na wcisniecie klawisza)
	
	LCD1602_ClearAll();					// Wyczysc ekran
	LCD1602_SetCursor(0,0);
	
	int k=0;
	
	while(1)		// Poczatek petli glównej
	{	
		while(!S2_press && !S3_press && !S4_press);
		if(S2_press && k<4) {
			morse_code_string[k++] = '-';
				sprintf(display,"%c",'-');
				LCD1602_Print(display);
				S2_press=0;
		}
		else if(S3_press && k<4) {
			morse_code_string[k++] = '.';
				sprintf(display,"%c",'.');
				LCD1602_Print(display);
				S3_press=0;
		}
		else if(S4_press) {
			for(int j = 0; j < 26; j++) {
				if(strcmp(morse_code_string, morse_code[j]) == 0) {
					sprintf(display,"%s",letters[j]);	
				}
			}
			LCD1602_Print(display);
			
			while(PTA->PDIR&S4_MASK);
			DELAY(3000)
			LCD1602_ClearAll();					// Wyczysc ekran
			LCD1602_SetCursor(0,0);
			memset(morse_code_string, 0x20, 4);
			memset(display, 0x20, 16);
			k=0;
			S4_press=0;
		}
	}
	
}


